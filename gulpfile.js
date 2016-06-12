'use strict';

var path = require('path');
var fs = require('fs');
var gulp = require('gulp');
var argv = require('yargs').default('environment', 'stage').argv;
var $ = require('gulp-load-plugins')();
var merge = require('merge-stream');
var runSequence = require('run-sequence');

var RELEASE_NAME = 'quirkbot-arduino-hardware';
var PACKAGE_INDEX_NAME = 'quirkbot.com';
var PACKAGE = JSON.parse(fs.readFileSync('package.json'));
var VERSION_FILENAME = `${RELEASE_NAME}-${PACKAGE.version}.zip`;
var LATEST_FILENAME = `${RELEASE_NAME}-latest.zip`;
var PACKAGE_INDEX_FILENAME = `package_${PACKAGE_INDEX_NAME}_index.json`;

/**
 * Cleans all the generated files
 */
gulp.task('clean', function () {
	return gulp.src([
		LATEST_FILENAME,
		RELEASE_NAME,
		RELEASE_NAME + '-*.zip'
	])
	.pipe($.clean());
});

/**
 * Gives a warning in case the current release you are building overwrites an
 * existing release.
 */
gulp.task('check-release-overwrite', [], function () {
	var overwrite = false;
	var oldIndex = JSON.parse(fs.readFileSync(PACKAGE_INDEX_FILENAME));
	var platforms = oldIndex.packages[0].platforms;
	for (var i = platforms.length - 1; i >= 0; i--) {
		if(platforms[i].version == PACKAGE.version){
			platforms.splice(i, 1);
			overwrite = true;
		}
	}
	if(overwrite){
		return gulp.src('')
		.pipe($.prompt.confirm(`There is already a release with the version ${PACKAGE.version}. Do you want to overwrite it (Y)?\n (If you want to create a new release, answer (N) and updated the version in package.json first)`))
	}

});

/**
 * Bundles the source code into a versioned zip file
 */
gulp.task('bundle:copy', function (gulpCallBack){
	return gulp.src([ 'avr/**' ])
	.pipe(gulp.dest(path.join(RELEASE_NAME, 'avr')))
});
gulp.task('bundle:zip', function (cb){
	var exec = require('child_process').exec;

	exec(
		`cd ${RELEASE_NAME}`
		+ ` && zip -vr ${VERSION_FILENAME} avr -x "*.DS_Store"`
		+ ` && cd ..`
		+ ` && mv  ${path.join(RELEASE_NAME, VERSION_FILENAME)} ${VERSION_FILENAME}`
		+ ` && cp  ${VERSION_FILENAME} ${LATEST_FILENAME}`
		+ ` && rm  -r ${RELEASE_NAME}`,
		(error, stdout, stderr) => {
			cb(error);
		}
	);

});
gulp.task('bundle', function (cb) {
	runSequence(
		'check-release-overwrite',
		'clean',
		'bundle:copy',
		'bundle:zip',
	cb);
});

/**
 * Generate the "platform" entry on the package index
 */
gulp.task('build', ['bundle'], function (cb) {


	// Check if there is already a release with this version
	var oldIndex = JSON.parse(fs.readFileSync(PACKAGE_INDEX_FILENAME));
	var platforms = oldIndex.packages[0].platforms;
	for (var i = platforms.length - 1; i >= 0; i--) {
		if(platforms[i].version == PACKAGE.version){
			platforms.splice(i, 1);
		}
	}

	// Compute the size and checksum of the bundle
	var execSync = require('child_process').execSync;
	var size = execSync(`stat -f%z ${VERSION_FILENAME}`).toString().replace(/\W/g, '');
	var checksum = execSync(`shasum -a 256 ${VERSION_FILENAME}`).toString()
	.split(VERSION_FILENAME).join('').replace(/\W/g, '');

	// Create the platform entry
	var newPlatform = fs.readFileSync('index_platform.template.json').toString()
	.split('{{VERSION}}').join(PACKAGE.version)
	.split('{{FILENAME}}').join(VERSION_FILENAME)
	.split('{{SIZE}}').join(size)
	.split('{{CHECKSUM}}').join(checksum);
	newPlatform = JSON.parse(newPlatform);

	// Apend the new platform entry to the current platforms on the package
	platforms.unshift(newPlatform);
	var newIndex = fs.readFileSync('index.template.json').toString()
	.split('{{PLATFORMS}}').join(JSON.stringify(platforms));
	newIndex = JSON.parse(newIndex);

	// Save the new index
	fs.writeFileSync(PACKAGE_INDEX_FILENAME, JSON.stringify(newIndex, null, '\t'));

	// Update the avr/version.txt
	fs.writeFileSync(path.join('avr', 'version.txt'), PACKAGE.version);

	cb();
});


/**
 * Builds and publish to s3
 */
gulp.task('s3', ['build'], function () {
	var aws = JSON.parse(fs.readFileSync(path.join('aws-config', `${argv.environment}.json`)));

	return gulp.src([
		PACKAGE_INDEX_FILENAME,
		VERSION_FILENAME,
		LATEST_FILENAME
	])
	.pipe($.s3(aws, {
		uploadPath: 'downloads/'
	}));
});


/**
 * Deploys the release. Asks for confirmation if deploying to production
 */
gulp.task('confirm-deploy', [], function () {
	if(argv.environment == 'production'){
		return gulp.src('')
		.pipe($.prompt.confirm('You are about to deploy TO PRODUCTION! Are you sure you want to continue)'))
		.pipe($.prompt.confirm('Really sure?!'))
	}

});
gulp.task('deploy', function (cb) {
	runSequence(
		'confirm-deploy',
		's3',
	cb);
});



rm -r s3_publish
mkdir s3_publish

#cp quirkbot-avr.tar.bz2 s3_publish/quirkbot-avr.tar.bz2
cp QuirkbotArduinoHardware.zip s3_publish/QuirkbotArduinoHardware.zip

aws s3 sync s3_publish s3://code.quirkbot.com/downloads/ --exclude *.DS_Store


rm -r s3_publish

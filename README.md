# Quirkbot Arduino Hardware
## Building releases
Build release files for integrating with Arduino's boards index and for manual distribution.

- Run ```$ sh build-release.sh```
- Copy the ```SIZE``` and ```CHECKSUM``` outputs.
- Update ```package_quirkbot.com_index.json``` with the copied ```SIZE``` and ```CHECKSUM``` using this format (make sure to also update the package version accordingly):

```
...
"version": "{UPDATED-VERSION-NUMBER}",
"archiveFileName": "quirkbot-avr.tar.bz2",
"checksum": "SHA-256:{CHECKSUM}",
"size": "{SIZE}",

...
```
- Host all the files and make sure to update the [Unofficial list of 3rd party boards support urls](https://github.com/arduino/Arduino/wiki/Unofficial-list-of-3rd-party-boards-support-urls).
- For manual distribution, use the generated `QuirkbotArduinoHardware.zip` file.

## Updating dependencies
### Libraries
- Servo
 - From git - https://github.com/arduino-libraries/Servo
- Keyboard
 - From git - https://github.com/arduino-libraries/Keyboard
- Mouse
 - From git - https://github.com/arduino-libraries/Mouse
- MIDIUSB
 - From git - https://github.com/arduino-libraries/MIDIUSB
- HID
 - Copied from *Arduino AVR Boards*.
 - Check what is the latest version at the [package index](http://downloads.arduino.cc/packages/package_index.json).

### Core
- Copied from *Arduino AVR Boards*.
- Check what is the latest version at the [package index](http://downloads.arduino.cc/packages/package_index.json).


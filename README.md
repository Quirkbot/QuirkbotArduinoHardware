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
*Make sure to keep this updated!*
### Arduino Core
Copied from *Arduino AVR Boards*.

Check what is the latest version at the [package index](http://downloads.arduino.cc/packages/package_index.json).

You need to patch the `D_CONFIG` macro from `USBCore.h` so it reports the board as self powered and with a maximum consumption of 100mA.
```
#define D_CONFIG(_totalLength,_interfaces) \
	{ 9, 2, _totalLength,_interfaces, 1, 0, USB_CONFIG_POWERED_MASK | USB_CONFIG_REMOTE_WAKEUP, USB_CONFIG_POWER_MA(100) }
```
### Libraries
##### HID
Copied from *Arduino AVR Boards*.

Check what is the latest version at the [package index](http://downloads.arduino.cc/packages/package_index.json).

##### Servo
From git - https://github.com/Quirkbot/Servo

##### Keyboard
From git - https://github.com/Quirkbot/Keyboard

##### Mouse
From git - https://github.com/Quirkbot/Mouse

##### MIDIUSB
From git - https://github.com/Quirkbot/MIDIUSB

You need to patch the `getShortName` function from `MIDIUSB.cpp` so it doesn't set any name (we don't want Arduino to set a serial number in the USB descriptors):
```
uint8_t MIDI_::getShortName(char* name)
{
    return 0;
}
```
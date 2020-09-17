# Quirkbot Arduino Hardware
## Deploying Releases

- Install node dependencies:
```
npm install
```
- Use node version 10 (tested with 14 and it doesn't work):
```
nvm use 10
```
- Update the version in `package.json`
- Run:
```
npm run gulp -- build
```
- When you are completely satisfied with the deployed release, run:
```
npm publish
```
- Create a git tag.
- Make a manual [Github release](https://github.com/Quirkbot/QuirkbotArduinoHardware/releases) and upload the generated _quirkbot-arduino-hardware-*.zip_.
- Make sure to update the [Unofficial list of 3rd party boards support urls](https://github.com/arduino/Arduino/wiki/Unofficial-list-of-3rd-party-boards-support-urls).

## Updating dependencies
*Make sure to keep them updated!*

### Arduino Core
Copied from *Arduino AVR Boards*.

Check what is the latest version at the [package index](http://downloads.arduino.cc/packages/package_index.json).

You need to patch the `D_CONFIG` macro from `USBCore.h` so it reports the board as self powered and with a maximum consumption of 100mA.
```
#define D_CONFIG(_totalLength,_interfaces) \
	{ 9, 2, _totalLength,_interfaces, 1, 0, USB_CONFIG_SELF_POWERED | USB_CONFIG_REMOTE_WAKEUP, USB_CONFIG_POWER_MA(100) }
```

You need to patch `CDC.cpp` auto-reset routine. Replace the content of lines 97 - 151 with:
```
if (CDC_SET_LINE_CODING == r || CDC_SET_CONTROL_LINE_STATE == r)
{
	if (1200 == _usbLineInfo.dwDTERate && (_usbLineInfo.lineState & 0x01) == 0)
	{
		*(uint16_t *)0x0800 = 0x7777;
		wdt_enable(WDTO_120MS);
		for (;;);
	}
	else
	{
		wdt_disable();
		wdt_reset();
	}
}
```

You need to patch `USBCore.cpp` `USB_Send` function. We want to remove the 250
timeout completely.

Replace the block:
```
u8 n = USB_SendSpace(ep);
if (n == 0)
{
    if (!(--timeout))
        return -1;
    delay(1);
    continue;
}
```
With:
```
u8 n = USB_SendSpace(ep);
if (n == 0)
{
    return -1;
}
```

### Libraries
##### HID
Copied from *Arduino AVR Boards*.`*`

You need to patch the `getShortName` function from `HID.cpp` so it doesn't set any name (we don't want Arduino to set a serial number in the USB descriptors):
```
uint8_t HID_::getShortName(char*)
{
    return 0;
}
```

##### Wire
Copied from *Arduino AVR Boards*.`*`

`(*)`Check what is the latest version at the [package index](http://downloads.arduino.cc/packages/package_index.json).

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
uint8_t MIDI_::getShortName(char*)
{
    return 0;
}
```

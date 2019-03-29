# Building the Bootloader

1. Make sure you have all the dependencies
  1.1. Make
  1.2. avr-gcc
    1.2.1 $ brew tap osx-cross/avr
          $ brew install avr-gcc
  1.3. SRecord
    1.3.1 $ brew install srecord
2. If you want to update Lufa, grab it from http://www.fourwalledcubicle.com/LUFA.php or  https://github.com/abcminiuser/lufa/releases
  2.1. Extract it as ./LUFA.
3. Open a command prompt in this directory.
4. Type 'sh build.sh'.
5. Enjoy!

# Flashing the Bootloader

1. Make sure you have all the dependencies
  1.1. avrdude
    1.1.1 $ brew install avrdude
2. Open a command prompt in this directory.
3. To flash with an "Arduino as ISP" on a Mac
  3.1. type 'sh flash-mac-arduino-isp.sh'
  3.2. The first time you run it, it will print the usb ports that are
       available and then it will fail (because the port will be unspecified).
       Copy, the correct port, then replace "usbmodemXXX" with it, run again.
4. To flash with on Windows
  4.1. type 'flash-avrispmkii'

#!/bin/bash
# check the correct serial port
ls /dev/tty.usb*
# set fuses
avrdude -v -patmega32u4 -cstk500v1 -P/dev/tty.usbmodemXXX -b19200 -e -Ulock:w:0x3F:m -Uefuse:w:0xce:m -Uhfuse:w:0xd8:m -Ulfuse:w:0xff:m
# flash bootloader
avrdude -v -patmega32u4 -cstk500v1 -P/dev/tty.usbmodemXXX -b19200 -Uflash:w:QuirkbotBootloader.hex:i

#!/bin/bash
avrdude -v -patmega32u4 -cstk500v1 -P/dev/cu.usbmodem1411 -b19200 -e -Ulock:w:0x3F:m -Uefuse:w:0xce:m -Uhfuse:w:0xd8:m -Ulfuse:w:0xff:m
avrdude -v -patmega32u4 -cstk500v1 -P/dev/cu.usbmodem1411 -b19200 -Uflash:w:QuirkbotBootloader.hex:i
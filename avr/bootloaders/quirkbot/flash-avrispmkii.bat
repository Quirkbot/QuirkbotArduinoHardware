avrdude -v -patmega32u4 -cavrispmkII -P usb -e -Ulock:w:0x3F:m -Uefuse:w:0xce:m -Uhfuse:w:0xd8:m -Ulfuse:w:0xff:m
avrdude -v -patmega32u4 -cavrispmkII -P usb -Uflash:w:QuirkbotBootloader.hex:i

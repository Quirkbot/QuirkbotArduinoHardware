# Build the bootloader
cd Bootloader
make clean
make
mv QuirkbotBootloader.hex ../QuirkbotBootloader.hex
make clean
cd ..

# Build the bootloader replacer program
rm -f QuirkbotBootloader.c
rm -f QuirkbotBootloaderUpdater.ino
srec_cat QuirkbotBootloader.hex \
-intel \
-crop 0 0xf000 \
-o QuirkbotBootloader.c \
-c-array \
bootloader_data \
-no-const
sed -i '' 's/unsigned char bootloader_data/const PROGMEM uint8_t bootloader_data/g' QuirkbotBootloader.c
cat QuirkbotBootloader.c QuirkbotBootloaderUpdater.ino.template >> QuirkbotBootloaderUpdater.ino

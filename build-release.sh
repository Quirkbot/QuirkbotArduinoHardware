rm -r quirkbot-arduino-hardware
mkdir quirkbot-arduino-hardware

cp -r avr quirkbot-arduino-hardware/avr

cp quirkbot-arduino-hardware/avr/bootloaders/caterina-Quirkbot/Caterina-Quirkbot.hex __Caterina-Quirkbot.hex

rm -r quirkbot-arduino-hardware/avr/bootloaders/caterina-Quirkbot/*

mv __Caterina-Quirkbot.hex quirkbot-arduino-hardware/avr/bootloaders/caterina-Quirkbot/Caterina-Quirkbot.hex

rm quirkbot-arduino-hardware.zip
cd quirkbot-arduino-hardware
zip -vr quirkbot-arduino-hardware.zip \
avr \
-x "*.DS_Store"
cd ..
mv quirkbot-arduino-hardware/quirkbot-arduino-hardware.zip quirkbot-arduino-hardware.zip

rm -r quirkbot-arduino-hardware

clear


echo
echo
echo 'SIZE'
stat -f%z quirkbot-arduino-hardware.zip
echo
echo
echo 'CHECKSHUM'
shasum -a 256 quirkbot-arduino-hardware.zip

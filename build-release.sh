
rm -r quirkbot-avr

cp -r avr quirkbot-avr
cp quirkbot-avr/bootloaders/caterina-Quirkbot/Caterina-Quirkbot.hex __Caterina-Quirkbot.hex
rm -r quirkbot-avr/bootloaders/caterina-Quirkbot/*
mv __Caterina-Quirkbot.hex quirkbot-avr/bootloaders/caterina-Quirkbot/Caterina-Quirkbot.hex
rm -r quirkbot-avr.tar.bz2
tar -jcf quirkbot-avr.tar.bz2 quirkbot-avr

rm QuirkbotArduinoHardware.zip
zip -vr QuirkbotArduinoHardware.zip \
quirkbot-avr \
-x "*.DS_Store"

rm -r quirkbot-avr

clear


echo
echo
echo 'SIZE'
stat -f%z quirkbot-avr.tar.bz2
echo
echo
echo 'CHECKSHUM'
shasum -a 256 quirkbot-avr.tar.bz2

rm QuirkbotArduinoHardware.zip
zip -vr QuirkbotArduinoHardware.zip \
avr \
-x "*.DS_Store"

clear

cp -r avr quirkbot-avr
rm -r quirkbot-avr/bootloaders/caterina-Quirkbot/LUFA-111009
rm -r quirkbot-avr.tar.bz2
tar -jcf quirkbot-avr.tar.bz2 quirkbot-avr
rm -r quirkbot-avr

echo
echo
echo 'SIZE'
stat -f%z quirkbot-avr.tar.bz2
echo
echo
echo 'CHECKSHUM'
shasum -a 256 quirkbot-avr.tar.bz2

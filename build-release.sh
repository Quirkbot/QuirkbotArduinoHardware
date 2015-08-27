rm -r QuirkbotArduinoHardware
mkdir QuirkbotArduinoHardware

cp -r avr QuirkbotArduinoHardware/avr

cp QuirkbotArduinoHardware/avr/bootloaders/caterina-Quirkbot/Caterina-Quirkbot.hex __Caterina-Quirkbot.hex

rm -r QuirkbotArduinoHardware/avr/bootloaders/caterina-Quirkbot/*

mv __Caterina-Quirkbot.hex QuirkbotArduinoHardware/avr/bootloaders/caterina-Quirkbot/Caterina-Quirkbot.hex

rm -r quirkbot-avr.tar.bz2
tar -jcf quirkbot-avr.tar.bz2 QuirkbotArduinoHardware/avr

rm QuirkbotArduinoHardware.zip
cd QuirkbotArduinoHardware
zip -vr QuirkbotArduinoHardware.zip \
avr \
-x "*.DS_Store"
cd ..
mv QuirkbotArduinoHardware/QuirkbotArduinoHardware.zip QuirkbotArduinoHardware.zip

rm -r QuirkbotArduinoHardware

clear


echo
echo
echo 'SIZE'
stat -f%z quirkbot-avr.tar.bz2
echo
echo
echo 'CHECKSHUM'
shasum -a 256 quirkbot-avr.tar.bz2

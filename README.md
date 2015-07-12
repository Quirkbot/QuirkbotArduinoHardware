# Quirkbot Arduino Hardware
### Intructions to build AVR boards
- Run ```$ sh build-package.sh```
- Copy the ```SIZE``` and ```CHECKSUM``` outputs.
- Update ```package_quirkbot.com_index.json``` with the copied ```SIZE``` and ```CHECKSUM``` using this format:

```
...

"archiveFileName": "quirkbot-avr.tar.bz2",
"checksum": "SHA-256:{CHECKSUM}",
"size": "{SIZE}",

...
```

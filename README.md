# Embedded_LAB (2nd Semester, 2564(2021))

## Materials
* Kidbright Version 1.3 (Using USB type A to micro USB)
  * ESP-WROOM-32
  * Electronic Design : https://www.kid-bright.org/download/1940/
* Arduino IDE

## Setting Arduino IDE for ESP32
1. Opening the Arduino IDE
2. Adding board manager in ```File >> Preferences``` and paste ```https://dl.espressif.com/dl/package_esp32_index.json``` on ```Additional Board Manager URLs```
3. Restart the Arduino IDE
4. Adding ESP32 on ```Tools >> Board: "..." >> Boards Manager...``` and type ```esp``` for finding
5. Install ```esp32```
6. Restart the Arduino IDE
7. Choosing ```Node32s``` on ```Tools >> Board: "..." >> Boards Manager...```

## Datasheets and References
* Temperature Sensor (LM73) : https://www.ti.com/lit/ds/symlink/lm73.pdf
* Real Time Clock (MCP79411) : http://ww1.microchip.com/downloads/en/devicedoc/20002266h.pdf
* Thing Speak website : https://thingspeak.com/
* Thing Speak library : https://github.com/mathworks/thingspeak-arduino
* LINE Notify website and API : https://notify-bot.line.me/en/
* LINE Notify library : https://github.com/TridentTD/TridentTD_LineNotify

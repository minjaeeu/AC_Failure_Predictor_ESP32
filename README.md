*Requires prior setup of https://github.com/minjaeeu/AC_Failure_Predictor_rest_server (RESTful server) to work.

*This project was developed with ESP32-DevKitC (https://docs.espressif.com/projects/esp-idf/en/latest/esp32/hw-reference/esp32/get-started-devkitc.html) board in mind.

*The following two sensors where used: DHT11 (temp/humidity) and SCT-013-020A (current).

Code was written using Arduino IDE version @2.2.1 on a ESP32-DevKitC module, the below configurations where used to flash the code:

CPU Frequency: 240MHz (WiFi/BT).
Core Debug Leve: None.
Erase All Flash Before Sketch Upload: Disabled.
Events Run On: Core 1.
Flash Frequency: 80MHz.
Flash Mode: DIO.
Flash Size: 4MB (32Mb).
JTAG Adapter: Disabled.
Arduino Runs On: Core 1.
Partition Scheme: Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS).
PSRAM: Disabled.
Upload Speed: 921600.


The below libraries where used:

ArduinoJson @7.0.4 - By Benoit Blanchon.
HTTPClient @2015 - By Espressif Systems.
time @2018 - By Espressif Systems.
WiFi @2014 - By Espressif Systems.
DHT sensor library @1.4.6 - By Adafruit.






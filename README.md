*Requires prior setup of https://github.com/minjaeeu/AC_Failure_Predictor_rest_server (RESTful server) to work.

*This project was developed with ESP32-DevKitC (https://docs.espressif.com/projects/esp-idf/en/latest/esp32/hw-reference/esp32/get-started-devkitc.html) board in mind.

*The following two sensors where used: DHT11 (temp/humidity) and SCT-013-020A (current).

Code was written using Arduino IDE version @2.2.1 on a ESP32-DevKitC module, the below configurations where used to flash the code:

CPU Frequency: 240MHz (WiFi/BT).<br />
Core Debug Leve: None.<br />
Erase All Flash Before Sketch Upload: Disabled.<br />
Events Run On: Core 1.<br />
Flash Frequency: 80MHz.<br />
Flash Mode: DIO.<br />
Flash Size: 4MB (32Mb).<br />
JTAG Adapter: Disabled.<br />
Arduino Runs On: Core 1.<br />
Partition Scheme: Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS).<br />
PSRAM: Disabled.<br />
Upload Speed: 921600.<br />


The below libraries where used:

ArduinoJson @7.0.4 - By Benoit Blanchon.<br />
HTTPClient @2015 - By Espressif Systems.<br />
time @2018 - By Espressif Systems.<br />
WiFi @2014 - By Espressif Systems.<br />
DHT sensor library @1.4.6 - By Adafruit.<br />






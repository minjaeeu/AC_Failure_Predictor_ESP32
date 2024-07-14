#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <time.h>
#include <DHT.h>
#include <EmonLib.h>

#define SCT013Pin 34  // GPIO pin for the SCT 013-20A-1V sensor
#define DHT11Pin 5    // GPIO pin for the DHT11 sensor
#define DHTType DHT11 
#define SamplingTime 2000 // Default sleep time in MS from which our loop will be halted after each sampling 

EnergyMonitor emon1;
double irms; // will hold the irms readings from emonlib through SCT013 sensor

DHT dht(DHT11Pin, DHTType);
float humidity, temp;  // will hold the readings from DHT11 sensor

const char* ssid = "XXXXXXXXXXXXXXX";      // network name, must be the same network as the API server
const char* password = "XXXXXXXXXXXXXXX";  // network password, must be the same network as the API server

char currentDateTime[20];  // holds the current datetime from the RTC as YYYY-MM-DD HH:MM:SS

String postEndpoint = "XXXXXXXXXXXXXXX";  // API endpoint for adding new entry
HTTPClient session;
JsonDocument json;
int httpResponseCode;  // holds the response code
char payload[256];     // holds the payload with sensors data



// sleeps using millis() for a certain amount of time in ms
void sleep(int sleepTime) {
  unsigned long startTime = millis();  // Get the current time in milliseconds
  while (millis() - startTime < sleepTime) {
  }
}

// returns current date time as YYYY-MM-DD HH:MM:SS
char* getDateTime(char* buffer) {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return nullptr;
  }
  snprintf(buffer, 20, "%04d-%02d-%02d %02d:%02d:%02d",
           timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday,
           timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  return buffer;
}

// configuring WiFi
void configureWiFi(void) {
  // configuring WiFi connection
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("waiting for connection...");
    sleep(500);
  }

  Serial.println("Connected !!!");
  Serial.print("IP:: ");
  Serial.println(WiFi.localIP());
}

// configuring RTC
void configureRTC(void) {
  configTime(-4 * 3600, 0, "pool.ntp.org");  // Manaus Time-Zone
  while (!time(nullptr)) {
    Serial.println("Waiting for time to be set...");
    sleep(1000);
  }
}


void setup() {
  Serial.begin(115200);

  configureWiFi();
  configureRTC();

  emon1.current(SCT013Pin, 20);  // Calibration for volta output sensors -> input/output 20A/1V = 20.
}

void loop() {

  // Taking the current value from SCT-013-20A-1V
  irms = emon1.calcIrms(1480); // 1480 == number of samples
  irms = irms/2; // On tests it was shown that the current readings from SCT-013-20A-1V 
                // where double than a standard clamp meter, hence the division.
  
  // Taking DTH11 samples (note that the sampling time for this sensor is 2 seconds)
  humidity = dht.readHumidity();
  temp = dht.readTemperature();

  // Check if any reads failed, if failed wait for 2 seconds to try again.
  while (isnan(humidity) || isnan(temp)) {
    Serial.println("Failed to read from DHT sensor!");
    Serial.println("Trying to read again...");
    sleep(2000);
    humidity = dht.readHumidity();
    temp = dht.readTemperature();
  }



  // sending sensor mock data to API
  if (WiFi.status() == WL_CONNECTED) {
    session.begin(postEndpoint);
    session.addHeader("Content-Type", "application/json");

    // // creating the payload
    json["datetime"] = String(getDateTime(currentDateTime));  // current date time as YYYY-MM-DD HH:MM:SS
    json["humidity"] = String(humidity, 2);                   // value from DTH11 sensor
    json["temperature"] = String(temp, 2);                    // value from DTH11 sensor
    json["current"] = String(irms, 2);                        // value from SCT-013-20A-1V sensor


    serializeJson(json, payload);                      // serializing the json into the payload char
    httpResponseCode = session.POST(String(payload));  // makes post request & passes response code to the variable at the left

    Serial.println("status code is: " + String(httpResponseCode));
    Serial.println("payload is: " + String(payload));
  }
  sleep(SamplingTime);  // sleeping for X seconds before doing loop run
}
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <time.h>

const char* ssid = "XXXXX";   // network name, must be the same network as the API server
const char* password = "XXXXX";  // network password, must be the same network as the API server

char currentDateTime[20];
char payload[256];

String postEndpoint = "XXXXXXXXXXXXXX";  // API endpoint for adding new entry
JsonDocument json;
HTTPClient session;

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
}

void loop() {
  // sending sensor mock data to API
  if (WiFi.status() == WL_CONNECTED) {
    session.begin(postEndpoint);
    session.addHeader("Content-Type", "application/json");

    // // creating the payload
    json["datetime"] = String(getDateTime(currentDateTime));  // current date time as YYYY-MM-DD HH:MM:SS
    json["humidity"] = 55.7576576;                            // value from humidity sensor
    json["temperature"] = 55.7333587986;                      // value from temp sensor
    json["wattage"] = 55.32336;                               // value from amp sensor


    serializeJson(json, payload);                          // serializing the json into the payload char
    int httpResponseCode = session.POST(String(payload));  // makes post request & passes response code to the variable at the left

    sleep(2000);  // sleeping for 2 second before doing another push
    Serial.println("status code is: " + String(httpResponseCode));
    Serial.println(payload);
  }
}
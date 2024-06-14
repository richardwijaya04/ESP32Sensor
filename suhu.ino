#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

#define DHT11_PIN  21 // ESP32 pin GPIO21 connected to DHT11 sensor

const char* ssid = "Tarigeng";
const char* password = "17022004";
const char* serverName = "http://192.168.1.37:5000/data";

DHT dht11(DHT11_PIN, DHT11);

void setup() {
  Serial.begin(115200);
  dht11.begin(); // initialize the DHT11 sensor

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  float humi = dht11.readHumidity();
  float tempC = dht11.readTemperature();

  if (isnan(tempC) || isnan(humi)) {
    Serial.println("Failed to read from DHT11 sensor!");
  } else {
    Serial.print("Humidity: ");
    Serial.print(humi);
    Serial.print("%  |  Temperature: ");
    Serial.print(tempC);
    Serial.println("°C");

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(serverName);
      http.addHeader("Content-Type", "application/json");

      String httpRequestData = "{\"humidity\":" + String(humi) + ",\"temperature\":" + String(tempC) + "}";
      int httpResponseCode = http.POST(httpRequestData);

      if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println(httpResponseCode);
        Serial.println(response);
      } else {
        Serial.print("Error on sending POST: ");
        Serial.println(httpResponseCode);
      }
      http.end();
    } else {
      Serial.println("WiFi Disconnected");
    }
  }

  delay(2000);
}

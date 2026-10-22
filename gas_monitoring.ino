// Smart Gas Monitoring: MQ gas sensor + ESP8266 -> ThingSpeak
// Wiring: MQ VCC -> 3.3V/5V, GND -> GND, A0 (analog out) -> ESP8266 A0

#include <ESP8266WiFi.h>

// ---- Fill these in before uploading. Never commit real credentials. ----
String apiKey = "YOUR_THINGSPEAK_WRITE_API_KEY";
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
// -------------------------------------------------------------------------

const char* server = "api.thingspeak.com";

int gasPin = A0;
WiFiClient client;

void setup() {
  Serial.begin(9600);
  pinMode(gasPin, INPUT);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

void loop() {
  int gasValue = analogRead(gasPin);  // Read MQ sensor (0-1023)

  Serial.print("Gas Value: ");
  Serial.println(gasValue);

  if (client.connect(server, 80)) {
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(gasValue);

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    Serial.println("Data sent to ThingSpeak");
  }

  client.stop();

  // ThingSpeak free account limit: one update every 15 seconds
  delay(15000);
}

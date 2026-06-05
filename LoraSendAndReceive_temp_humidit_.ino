#include <MKRWAN.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11
#define BATTERY_PIN A6
#define WIND_PIN A2
DHT dht(DHTPIN, DHTTYPE);
LoRaModem modem;

#include "arduino_secrets.h"

String appEui = SECRET_APP_EUI;
String appKey = SECRET_APP_KEY;

void setup() {

  Serial.begin(115200);
  dht.begin();

  while (!Serial);

  if (!modem.begin(EU868)) {
    Serial.println("Failed to start module");
    while (1);
  }

  modem.joinOTAA(appEui, appKey);
  modem.minPollInterval(120);
}

void loop() {

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  int battValue = analogRead(BATTERY_PIN);
  float batteryVoltage = battValue * (3.3 / 1023.0) * 2.0;

  int windValue = analogRead(WIND_PIN);
  float windVoltage = windValue * (3.3 / 1023.0);
  float windSpeed = (windVoltage / 3.3) * 30.0;


  Serial.print("Temperature: ");
  Serial.println(temperature);

  Serial.print("Humidity: ");
  Serial.println(humidity);

  Serial.print("Battery: ");
  Serial.println(batteryVoltage);
    
  Serial.print("Vitesse vent : ");
  Serial.println(windSpeed);


  int temp = (int)temperature;
  int hum  = (int)humidity;
  int batt = (int)(batteryVoltage * 100);
  int vent = (int)windSpeed;

  byte payload[3];
  payload[0] = temp;
  payload[1] = hum;
  payload[2] = batt;
  payload[3] = vent;

  modem.beginPacket();
  modem.write(payload, 3);
  modem.endPacket(true);

  delay(120000);
}

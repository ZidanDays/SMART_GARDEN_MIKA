/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID           "TMPL6jyM_6Pbf"
#define BLYNK_TEMPLATE_NAME         "Quickstart Template"
// #define BLYNK_AUTH_TOKEN            "csi0MODBUXf6FHfNhzYZwbcZslUAxHa4"


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>


#define DHTTYPE DHT11 // DHT 11
DHT dht(D2, DHTTYPE);
char auth[] = "csi0MODBUXf6FHfNhzYZwbcZslUAxHa4";//Enter your Auth token
char ssid[] = "MyPublicWiFi";//Enter your WIFI name
char pass[] = "12345678";//Enter your WIFI password

BlynkTimer timer;
bool Relay = 0;

#define MOISTURE_THRESHOLD 30 // Set your desired moisture threshold here


//Define component pins
#define sensor A0
#define waterPump D3

// #define SENSOR_PIN_1 D0
// #define SENSOR_PIN_2 D1
// #define SENSOR_PIN_3 D2

void setup() {
  Serial.begin(9600);
  pinMode(waterPump, OUTPUT);
  digitalWrite(waterPump, HIGH);


  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);


  //Call the function
  timer.setInterval(100L, soilMoistureSensor);
}

//Get the button value
BLYNK_WRITE(V1) {
  Relay = param.asInt();

  if (Relay == 1) {
    digitalWrite(waterPump, LOW);
  } else {
    digitalWrite(waterPump, HIGH);
  }
}

//Get the soil moisture values
void soilMoistureSensor() {
  int value = analogRead(sensor);
  value = map(value, 0, 1024, 0, 100);
  value = (value - 100) * -1;

  // int value2 = digitalRead(SENSOR_PIN_1);
  // value2 = map(value2, LOW, HIGH, 0, 100);

  // int value3 = digitalRead(SENSOR_PIN_2);
  // value3 = map(value3, LOW, HIGH, 0, 100);
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  Blynk.virtualWrite(V0, value);
  // Blynk.virtualWrite(V3, value2);
  // Blynk.virtualWrite(V2, value3);
    Blynk.virtualWrite(V3, h);
  Blynk.virtualWrite(V4, t);

    // Check if soil moisture is below the threshold
  if (value < MOISTURE_THRESHOLD) {
    // Soil is dry, turn on the water pump
    digitalWrite(waterPump, LOW);
    Blynk.virtualWrite(V2, "Soil is dry. Watering...");
  } else {
    // Soil is wet, turn off the water pump
    digitalWrite(waterPump, HIGH);
    Blynk.virtualWrite(V2, "Soil is wet. Not watering.");
  }

}

void loop() {
  Blynk.run();//Run the Blynk library
  timer.run();//Run the Blynk timer
}
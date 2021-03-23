// Import required libraries
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>
#include <ErriezMHZ19B.h>
#include "index.h"





// Replace with your network credentials
const char* ssid = "SSID";
const char* password = "PW";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);


#define DHTPIN  D1    // Digital pin connected to the DHT sensor (D1 PIN)

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11


DHT dht(DHTPIN, DHTTYPE);

// current temperature & humidity, updated in loop()
float t = 0.0;
float h = 0.0;

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;    // will store last time DHT was updated

// Updates DHT readings every 10 seconds
const long interval = 10000;  


void readDHTTemperature(float &t) {

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you updated the DHT values
    previousMillis = currentMillis;
    // Read temperature as Celsius (the default)
    float newT = dht.readTemperature();

    if (isnan(newT)) {
      Serial.println("Failed to read from DHT sensor!");
    }
    else {
      t = newT;
      Serial.println(t);
     }
 }
}

void readDHTHumidity(float &hum) {
      // Read Humidity
    float newH = dht.readHumidity();
    // if humidity read failed, don't change h value 
    if (isnan(newH)) {
      Serial.println("Failed to read from DHT sensor!");
    }
    else {
      h = newH;
      Serial.println(h);
    }
}

// Water Temp


#define ONE_WIRE_BUS 2 // DS18B20 on arduino pin2 corresponds to D4 on physical board "D4 pin on the ndoemcu Module"
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);


String readDSTemperatureC() {
  float tempC;
  DS18B20.requestTemperatures();
  tempC = DS18B20.getTempCByIndex(0);

  if(tempC == -127.00) {
    Serial.print("Failed to read from DS18B20 sensor ");
    Serial.println(tempC); 
    //return "--";
  } else {
    Serial.print("Temperature Celsius: ");
    Serial.println(tempC); 
  }
  return String(tempC);
}

//Water Level Sensor
int sensorpin = A0;


String readWaterLevel(){
  int sensorval = 0;
  sensorval = analogRead(sensorpin);
  return String(sensorval);
  Serial.println(sensorval);
  }


//CO2

// Pin defines
    #define MHZ19B_TX_PIN        D5
    #define MHZ19B_RX_PIN        D6

SoftwareSerial mhzSerial(MHZ19B_TX_PIN, MHZ19B_RX_PIN);

// Create MHZ19B object
ErriezMHZ19B mhz19b(&mhzSerial);

String readCO2(){
if(mhz19b.isWarmingUp()){
  Serial.println(F("Warming up..."));
  return "0";
  }

else {
  if(mhz19b.isReady()){
  int16_t ppm = mhz19b.readCO2();
  Serial.print("CO2 Wert : ");
  Serial.println(ppm);
  return String(ppm);
    
  }
  else {
    return "-1";
    }
  
  }


}


void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);

   dht.begin(); //DHT11 Sensor starten
   DS18B20.begin(); // Start up the DS18B20 library

    // Initialize senor software serial at fixed 9600 baudrate
    mhzSerial.begin(9600);
   



  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
   Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Route for root / web page
   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

 
   
   server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(t).c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(h).c_str());
  });  
    server.on("/watertemp", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDSTemperatureC().c_str());
    });

    server.on("/waterlevel", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readWaterLevel().c_str());
    });
     server.on("/co2", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readCO2().c_str());
    });
    
    

  // Start server
  server.begin();
}



void loop() {
    
    //Read Room Temp
    readDHTTemperature(t);

     //Read Humidity    
    readDHTHumidity(h);

    delay(5000);
 

   

    
     
 

}

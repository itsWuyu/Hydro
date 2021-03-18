#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <UniversalTelegramBot.h>
#include "NewPing.h"  
#include <SPI.h>
#include <RCSwitch.h>



// Initialize sensor that uses digital pins 13 and 12.
int trigPin = D0;      // trigger pin
int echoPin = D1;      // echo pin


NewPing sonar(trigPin, echoPin);
RCSwitch mySwitch = RCSwitch();

//------- WiFi Settings -------
char ssid[] = "YOUR ID";       // your network SSID (name)
char password[] = "YOUR PW";  // your network key

#define BOTtoken "BOT ID" // Change it to your bot token

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int Bot_mtbs = 1000; //mean time between scan messages
long Bot_lasttime;   //last time messages' scan has been done


void bot_setup() {
  client.setInsecure(); // Required for ESP8266
  const String commands = F("["
    "{\"command\":\"help\",  \"description\":\"Get bot usage help\"},"
    "{\"command\":\"start\", \"description\":\"Message sent when you open a chat with a bot\"},"
    "{\"command\":\"status\",\"description\":\"Answer device current status\"}"  // no comma on last command
  "]");
  bot.setMyCommands(commands);
  bot.sendMessage("CHAT ID", "system running!", "Markdown");
}


String ipAddress = "";
bool again = true;


void setup () {
  Serial.begin(115200);
  Serial.print("Connecting Wifi: ");
  WiFi.begin(ssid, password); // Using stored credentials
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
     Serial.println("");
   Serial.println("WiFi connected");
   Serial.println("IP address: ");
   IPAddress ip = WiFi.localIP();
   Serial.println(ip);

  ipAddress = ip.toString();
    delay(500);
  }
  Serial.println("\r\nWiFi connected");
  ArduinoOTA.begin();   // You should set a password for OTA. Ideally using MD5 hashes
  bot_setup();

     
   // Transmitter is connected to Arduino Pin #11  
    mySwitch.enableTransmit(D3);

    // Optional set pulse length.
    mySwitch.setPulseLength(505);
    
    // Optional set protocol (default is 1, will work for most outlets)
    mySwitch.setProtocol(5);
      
    // Optional set number of transmission repetitions.
    //mySwitch.setRepeatTransmit(5);

  /// First measurement are sometimes off so we do these befor the real code starts
   for (int i = 0; i < 10; i++){
      sonar.ping_cm();    
      delay(50);
    }

   
}



void loop () {

    ArduinoOTA.handle(); 
    
     // Every 100 miliseconds, do a measurement using the sensor and print the distance in centimeters.
    //double distance = sonar.ping_cm();
    double mm[9] = {};

	for(int i = 0; i < 9; i++){	
	mm[i] = sonar.ping_cm();
  delay(100);
	}
	std::sort(mm, mm+5);
  for (int i = 0 ; i < 9; i++){
    Serial.print(mm[i]);
    Serial.print(" ");
    }
  
  double distance = mm[5];
  	 
 
  

    if (distance <= 10 && distance > 0 ){
       again = true;
       Serial.print("Distance: ");
       Serial.print(distance, 1); Serial.println(" cm");
       if(count >= 1 && count < 3){
        mySwitch.send("001000101000110101000111");
        }
       delay(1000);
       count = 0;
       
       
      
      }
   else if((distance > 10 || distance <= 0) && again == true ) {
   
      Serial.println("ERROR ");
      Serial.print("Distance: ");
      Serial.print(distance, 1); Serial.println(" cm");
      mySwitch.send("001010110000111011100111");
      count++;
      Serial.println(count);
      delay(1000);
    if(count == 3){
      for (int i = 0; i < 3; i++){
      bot.sendMessage("-529143841", "SCHLAUCH AB !! ERROR !! PUMPE DEAKTIVIERT !! *" + String(distance, 1) + "* cm ", "Markdown");
      delay(1000);
      }
    again = false;
    Serial.println("False");
    }
    }
    else{
      }
      
      

}

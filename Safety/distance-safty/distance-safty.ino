#include <HCSR04.h>
#include <SPI.h>
#include <RCSwitch.h>


// Initialize sensor that uses digital pins 13 and 12.
int triggerPin = 9;
int echoPin = 10;


UltraSonicDistanceSensor distanceSensor(triggerPin, echoPin);
RCSwitch mySwitch = RCSwitch();

void setup () {
    Serial.begin(9600);  // We initialize serial connection so that we could print values from sensor.
    // Transmitter is connected to Arduino Pin #11  
    mySwitch.enableTransmit(11);

    // Optional set pulse length.
    mySwitch.setPulseLength(505);
    
    // Optional set protocol (default is 1, will work for most outlets)
    mySwitch.setProtocol(5);
      
    // Optional set number of transmission repetitions.
    //mySwitch.setRepeatTransmit(5);

}

void loop () {
    // Every 500 miliseconds, do a measurement using the sensor and print the distance in centimeters.
    double distance = distanceSensor.measureDistanceCm();

    if (distance <= 10 && distance > 0 ){
       Serial.println(distance);
       delay(100);
       
      
      }
   else {
    Serial.println("ERROR");
    mySwitch.send("001010110000111011100111");
    }

}

#include <Adafruit_VCNL4010.h>

#include <Wire.h>
#include "Adafruit_VCNL4010.h"
#include "OneButton.h"                              //we need the OneButton library

#define SENSE A0
#define buzzer 6

long int b = 0;
int a = 0;

Adafruit_VCNL4010 vcnl;

bool state;

int pin = 11;
 
OneButton button(12, true);                         //attach a button on pin 12 to the library
 
void setup() {

Serial.begin(9600);
Serial.println("VCNL4010 test");
pinMode(buzzer, OUTPUT);

pinMode(pin, OUTPUT);
digitalWrite(pin, HIGH);

tone(buzzer, 1000);
delay(1000);
noTone(buzzer);

// check the proximity sensor connections
  if (! vcnl.begin()){
    Serial.println("Sensor not found :(");
    while (1);
  }
  Serial.println("Found VCNL4010");
  delay(1000);

  button.attachDoubleClick(doubleclick);            // link the function to be called on a doubleclick event.
  button.attachClick(singleclick);                  // link the function to be called on a singleclick event.
  button.attachLongPressStop(longclick);            // link the function to be called on a longpress event.

}


void loop() {
  
button.tick(); 

//   Serial.print("Ambient: "); Serial.println(vcnl.readAmbient());
 //  Serial.print("Proximity (a): "); Serial.println(vcnl.readProximity());  
   b = analogRead(SENSE);
   b = map(b, 0, 1023, 30000, 1000);   
//   Serial.print("Sensor level (b): "); Serial.println(b);

   a = vcnl.readProximity();
 //  Serial.print("Proximity (a): "); Serial.println(a); 
   
   //blink check
   if(a > b) {
 //  Serial.println("Blink detected");
   digitalWrite(pin, LOW);
   }
   else {
   digitalWrite(pin, HIGH);
        } 

delay(10);
}


void doubleclick() {                                // what happens when button is double-clicked
      tone(buzzer, 1000);
      delay(150);
      noTone(buzzer);       
      tone(buzzer, 1000);  
      delay(150);    
      noTone(buzzer);                             
}
 
void singleclick(){                                 // what happens when the button is clicked
      tone(buzzer, 1000);
      delay(300);
      noTone(buzzer);                              
}
 
void longclick(){                                   // what happens when buton is long-pressed
      tone(buzzer, 1000);
      delay(1000);
      noTone(buzzer);                          
}


      

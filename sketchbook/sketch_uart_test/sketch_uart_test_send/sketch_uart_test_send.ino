#include <AltSoftSerial.h>

AltSoftSerial mySerial; 
// Board          Transmit  Receive   PWM Unusable
// -----          --------  -------   ------------
// Arduino Uno        9         8         10

int LED = 0;
void setup() {
// Serial.begin(9600);
mySerial.begin(9600);
pinMode(13,OUTPUT);
}

void loop() { 
  if(mySerial.write(11))
    if(!LED){
      digitalWrite(13,true);
      LED=1;
    }
    else{
      digitalWrite(13,false);
      LED=0;
    }
  
  delay(2000);
}

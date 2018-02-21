#include <AltSoftSerial.h>
// AltSoftSerial always uses these pins:
// Board          Transmit  Receive   PWM Unusable
// -----          --------  -------   ------------
// Arduino Uno        9         8         10

AltSoftSerial mySerial; 

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  delay(10);
}

void loop() { 
  int in = 0;
  if (mySerial.available()) {
    Serial.print("received:");
    in = mySerial.read();
    Serial.println(in,DEC);
    
  }
  delay(1000);

}

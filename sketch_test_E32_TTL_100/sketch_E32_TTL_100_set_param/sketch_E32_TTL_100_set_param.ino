/*2018.1.25  huitian agrismarter.com
   Chen Jiahao
   test lora module E32-TTL-100 on arduino nano
   sending node
*/
#include <AltSoftSerial.h>

AltSoftSerial uart;
// Board          Transmit  Receive   PWM Unusable
// -----          --------  -------   ------------
// Arduino Uno        9         8         10

const int LED = 13;
const int M0 = 5;  //mode control 0
const int M1 = 6;  //mode control 1
const int AUX = 7;  //AUX detecting
int auxValue = 0;
uint8_t buf[60];   //command buffer
int sendNum = 0;
int receive = 0;

void setup() {
  // set up usb serial and software serial
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  uart.begin(9600);

  // set up pins
  pinMode(LED, OUTPUT);
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(AUX, INPUT);

  //wait till lora module is ready
  while (!auxValue) {
    auxValue = digitalRead(AUX);
    Serial.print("AUX:");
    Serial.println(auxValue);
  }

  //set to mode3 
  digitalWrite(M0, 1);
  digitalWrite(M1, 1);
  delay(10);

  //setting up the parameters in mode3: C0(not volatile) + 5 bytes of parameters
  buf[0] = 0xC0;  //HEAD
  buf[1] = 0x0 ;   //ADDH
  buf[2] = 0x1  ;  //ADDL
  buf[3] = 0x1A ;  //SPED
  buf[4] = 0x17 ;  //CHAN
  buf[5] = 0x44  ; //OPTION
  if (uart.write(buf, 6))
    Serial.println("Parameters have been set up.");
  delay(10);

  //validate parameters
  for (int i = 0; i++; i < 3)
    buf[i] = 0xC1;
  delay(10);

}

void loop() {
  Serial.println("receiving...");
  while (uart.available()) {
    Serial.print("received:");
    receive = uart.read();
    Serial.println(receive, HEX );
  }
  delay(2000);
  //  while(1);
}

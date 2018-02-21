/*2018.1.25  huitian agrismarter.com
   Chen Jiahao
   test lora module E32-TTL-100 on arduino nano
      receiving node
*/
#include <AltSoftSerial.h>

AltSoftSerial uart;
// Board          Transmit  Receive   PWM Unusable
// -----          --------  -------   ------------
// Arduino Uno        9         8         10

const int M0 = 5;  //mode control 0
const int M1 = 6;  //mode control 1
const int AUX = 7;  //AUX detecting
int auxValue = 0;
int receive = 0;
uint8_t buf[6];   //command buffer
//private function declaration
boolean loraReady(void);

void setup() {
  // set up usb serial and software serial
  uart.begin(9600);
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // set up pins
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(AUX, INPUT);

  //set to mode3 setting up
  while (!loraReady());//wait till lora module ready
  digitalWrite(M0, 1);
  digitalWrite(M1, 1);
  delay(10);

  //setting up the parameters in mode3: C0 + 5 bytes of parameters
  buf[0] = 0xC0;  //HEAD (none volatile)
  buf[1] = 0x0 ;   //ADDH  listenning to all addresses
  buf[2] = 0x2  ;  //ADDL
  buf[3] = 0x1A ;  //SPED
  buf[4] = 0x17 ;  //CHAN
  buf[5] = 0x44  ; //OPTION
  while (!loraReady()); //wait till lora module ready
  if (uart.write(buf, 6))
    Serial.println("Parameters have been set up.");
  delay(10);

  //set to mode0 transmitting
  while (!loraReady()); //wait till lora module ready
  digitalWrite(M0, 0);
  digitalWrite(M1, 0);
  Serial.println("Mode 0.");
  delay(10);

}

void loop() {
  // put your main code here, to run repeatedly:
  //get data from lora module
  Serial.println("receiving...");
  if (loraReady()) {
    while (uart.available()) {
      Serial.print("received:");
      receive = uart.read();
      Serial.println(receive, HEX );
    }
    delay(800);
  }
}

//check if lora module is busy
boolean loraReady(void) {
  auxValue = digitalRead(AUX);
  Serial.print("AUX:");
  Serial.println(auxValue);
  return auxValue ? true : false;
}


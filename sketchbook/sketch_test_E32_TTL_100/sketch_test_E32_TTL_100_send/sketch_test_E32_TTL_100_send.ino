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
uint8_t buf[58];   //command buffer
int sendNum = 0;
int receive = 0;
//private function declaration
boolean loraReady(void);

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

  //set to mode3 setting up
  while (!loraReady());//wait till lora module ready
  digitalWrite(M0, 1);
  digitalWrite(M1, 1);
  delay(10);

  //setting up the parameters in mode3: C0 + 5 bytes of parameters
  buf[0] = 0xC0;  //HEAD (not volatile)
  buf[1] = 0x0 ;   //ADDH
  buf[2] = 0x1  ;  //ADDL
  buf[3] = 0x1A ;  //SPED
  buf[4] = 0x17 ;  //CHAN
  buf[5] = 0x44  ; //OPTION
  while (!loraReady());//wait till lora module ready
  if (uart.write(buf, 6))
    Serial.println("Parameters have been set up.");
  delay(10);

  //set to mode0 transmitting
  while (!loraReady());//wait till lora module ready
  digitalWrite(M0, 0);
  digitalWrite(M1, 0);
  Serial.println("Mode 0.");
  delay(10);

  //fill buffer
  buf[0] = 0xFF;
  buf[1] = 0xFF;  //broadcasting
  buf[2] = 0x17;  //channel
  for (int i = 3; i < 58; i++) //data
    buf[i] = i;
}

void loop() {
  // put your main code here, to run repeatedly:
  //send commands to lora module
  sendNum = 58;
  if (loraReady()) {
    uart.begin(9600);
    delay(10);
    if (sendNum == uart.write(buf, 58)) {
      digitalWrite(LED, HIGH);
      Serial.print( "sent: ");
      Serial.print(sendNum, DEC);
      Serial.print( " contents: ");
      Serial.print(buf[0], HEX);
      Serial.print(buf[1], HEX);
      Serial.println(buf[2], HEX);
    }
    uart.end();
    delay(1000);
    digitalWrite(LED, LOW);
  }

  //get data from lora module
  //  Serial.println("receiving...");
  //  while (uart.available()) {
  //    Serial.print("received:");
  //    receive = uart.read();
  //    Serial.println(receive, HEX );
  //  }
  delay(1000);

}

//check if lora module is busy
boolean loraReady(void) {
  auxValue = digitalRead(AUX);
  Serial.print("AUX:");
  Serial.println(auxValue);
  return auxValue ? true : false;
}

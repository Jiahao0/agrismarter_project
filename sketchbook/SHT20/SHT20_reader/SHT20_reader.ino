// SHT20 reader
//by jiahao chen on 2018.2.9
//SDA: A4; SCL: A5

#include <SHT20_reader.h>

void setup() {
  Wire.begin();

  Serial.begin(9600);
  while (!Serial);             // Leonardo: wait for serial monitor
  Serial.println("\nSHT20 reader");
  delay(100);

  //soft reset
  if (softReset() == 0) {
    Serial.println("Soft reset!");
    delay(20);
  }
}

void loop() {

  //read register
  //  unsigned char val;
  //  if (readReg(&val) > 0) {
  //    Serial.print("read register: ");
  //    Serial.println(val, HEX);
  //  }
  //  //write register
  //  Serial.println("write register:");
  //  if ((val = writeReg( MODE_RH10_T13 )) == 0) {
  //    Serial.println("success!");
  //  }
  //  delay(100);
  //  //read again
  //  if (readReg(&val) > 0) {
  //    Serial.print("read register again: ");
  //    Serial.println(val, HEX);
  //  }
  //  unsigned char buf[] = {0x11, 0x44}; // 00010010, 00110100
  //  short value = (buf[0] << 8) | (buf[1]); //0001001000110100
  //  Serial.println(value, HEX); //0x1234

  //temperature mesurement
  unsigned short temp;
  if (measurement(&temp, T_MES_HOLD_MASTER)) {
    Serial.print("temperature: ");
    Serial.println(tempConvertion(temp));
  }
  //humidity
  if (measurement(&temp, RH_MES_HOLD_MASTER)) {
    Serial.print("humidity: ");
    Serial.println(humidConvertion(temp));
  }
  delay(1000);
}

//////////////////////////////////////// soft reset to sensor/////////////////////////////////
char softReset() {
  Wire.beginTransmission(ADDRESS); // transmit to device #64 (0x40)
  Serial.println(Wire.write(SOFT_RESET));
  char st = Wire.endTransmission();
  Serial.println( st, HEX );
  return st;     // stop transmitting
}

/////////////////////////////////////////////set up user register//////////////////////////////////
char writeReg(char valToWrite) {
  unsigned char buf[2];
  buf[0] = WRITE_USER_REG;
  unsigned char val;

  //read the register to get the reserved bits
  if (readReg( &val) > 0) {
    //    Serial.println(val, HEX); // 0x3A = 0011 1010
    val &= RESERVED_BITS_MASK; // 0011 1010 & 0011 1000 = 0011 1000 = 0x38
    //    Serial.println(val, HEX); //
    buf[1] = valToWrite | val; //value to send  0011 1000 | 10000011 = 1011 1011 = 0xBB
    //    Serial.println(buf[1], HEX);

    Wire.beginTransmission(ADDRESS); // transmit to device #64 (0x40)
    Serial.print("write number: ");
    Serial.println(Wire.write(buf, 2));           // sends value bytes
    return Wire.endTransmission();     // stop transmitting
  }
  else {
    Serial.println("error reading register");
    return -1;
  }

}

/////////////////read contents from user register/////////////////////////////////////////////////
int readReg(char *reg) {
  int error, num = 0;
  Wire.beginTransmission(ADDRESS); // transmit to device #64 << 1 (0x80)
  Wire.write(READ_USER_REG);            // sends value byte

  if ( (error = Wire.endTransmission(false)) == 0) {
    num = Wire.requestFrom(ADDRESS, 1, false);    // request 1 bytes from slave device
    if (num > 0)
      while (Wire.available())   // slave may send less than requested
        *reg = Wire.read();    // receive a byte as character
  }
  else {
    Serial.print("error trasmitting data!");
    Serial.println(errorHandler(error));
  }
  return num;
}

/////////////////////////////////////trigger a measurement/////////////////////////////
boolean measurement(unsigned short *value , unsigned char cmd) {
  boolean state = false;
  byte num = 0, error;
  unsigned char buf[2];
  Wire.beginTransmission(ADDRESS); // transmit to device #64 << 1 (0x80)
  Wire.write(cmd);            // sends value byte

  if ( (error = Wire.endTransmission(false)) == 0) {
    num = Wire.requestFrom(ADDRESS, 2, false);    // request 2 bytes from slave device
    char i = 0;
    if (num > 0) {
      state = true;
      while (Wire.available() && i < 2) { // slave may send less than requested
        buf[i++] = Wire.read();
        //        Serial.println(buf[i++], HEX);
      }
    }
    else {
      Serial.print("error trasmitting data!");
      Serial.println(errorHandler(error));
    }
    *value = (buf[0] << 8) | (buf[1]);
    //    Serial.println(*value, DEC);
    return state;
  }
}

////////////////////////////error handling////////////////////////////////////////////
char* errorHandler(char errorCode) {
  switch (errorCode) {
    case 0: return "SUCCESS";
    case 1: return "BUFFER_FULL";
    case 2: return "NACK_ADDR";
    case 3: return "NACK_DATA";
    default: return "UNKNOWN";
  }
}

//////////////////////////convertion temperature////////////////////////////////////
float tempConvertion(unsigned short val) {
  return -46.85 + (175.72 * (val & TEMPERATURE_STATUS)) / 65536;
}

////////////////////////////////convert humidity////////////////////////////////////
float humidConvertion(unsigned short val) {
  return -6 + (125.0 * (val & HUMIDITY_STATUS)) / 65536;
}


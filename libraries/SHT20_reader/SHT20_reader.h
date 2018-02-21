// SHT20 reader
//by jiahao chen on 2018.2.9
//SHT20_reader.h

//library of arduino
#include <Wire.h>

//user register settings
#define RESERVED_BITS_MASK	0x38 //00111000
#define MODE_RH12_T14 		0x02 //00000010  default
#define MODE_RH8_T12  		0x03 //00000011
#define MODE_RH10_T13 		0x82 //10000010
#define MODE_RH11_T11 		0x83 //10000011
#define OTP_ENABLE    		0xFD //11111101  enable OTP by setting bit 1 to 0, not recommended 

//commands for read/write user register or measurements
#define T_MES_HOLD_MASTER     0xE3 //11100011
#define T_MES_NO_HOLD_MASTER  0xF3 //11110011
#define RH_MES_HOLD_MASTER    0xE5 //11100101
#define RH_MES_NO_HOLD_MASTER 0xF5 //11110101
#define WRITE_USER_REG        0xE6 //11100110
#define READ_USER_REG         0xE7 //11100111
#define SOFT_RESET            0xFE //11111110

//address
#define ADDRESS 	0x40


//status bits of mesurements
#define TEMPERATURE_STATUS	0xFFFC
#define HUMIDITY_STATUS		0xFFFE

//private functions definitions
// soft reset to sensor
char softReset(void);
//set up user register
char writeReg(char);
//read register
int readReg(char *);
//trigger a measurement
boolean measurement(unsigned short *, unsigned char);
//error handling
char* errorHandler(char );
//convert temperature
float tempConvertion(unsigned short );
//convert humidity
float humidConvertion(unsigned short );

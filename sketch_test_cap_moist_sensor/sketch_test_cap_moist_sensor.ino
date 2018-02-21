/*2018.1.23  huitian agrismarter.com
 * Chen Jiahao
 * test a capacitive moisture sensor on arduino nano
 */

int sensorPin = A0;    // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor

void setup() {
  // put your setup code here, to run once:
  //start serial connection
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
    // stop the program for 1000 milliseconds:
  delay(1000);
    //print out the value of the pushbutton
  Serial.println(sensorValue);
}

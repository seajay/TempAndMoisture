// Code from http://arduino.cc/forum/index.php?topic=22114.0;wap2

const int LM35pin = A0; // connect LM35 Vout pin to arduino analog pin 0
const int LM35ref = A1; // connect 2x 1N1418 diodes between LM35 ground pin and ground
float LM35tempC;

const int HumidPowerA = 12;
const int HumidPowerB = 13;
const int HumidIn = A5;
int HumidInA;
int HumidInB;
const int referenceResistance= 22; // In kOhms, this is only a 16bit system.
float measuredResistance;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 
  pinMode(HumidPowerA, OUTPUT);
  pinMode(HumidPowerB, OUTPUT);
  pinMode(HumidIn, INPUT);
}

void loop() {

  // Read the Resistance of the moisture sensor
  digitalWrite(HumidPowerA, HIGH);
  digitalWrite(HumidPowerB, LOW);
  delay(10);
  HumidInA = analogRead(HumidIn);
  
  // Now switch polarity and read it again 
  // (this is to avoid electrolisis on the sensor)
  digitalWrite(HumidPowerA, LOW);
  digitalWrite(HumidPowerB, HIGH);
  delay(10);
  HumidInB = analogRead(HumidIn);
  
  // Switch off current through the sensor
  digitalWrite(HumidPowerB, LOW);
  
  measuredResistance = referenceResistance * (1023 - HumidInA)/HumidInA;
  Serial.print("R = ");
  Serial.print(measuredResistance);
  measuredResistance = referenceResistance * HumidInB / (1023 - HumidInB);
  Serial.print(" R = ");
  Serial.print(measuredResistance);
  
  LM35tempC = readLM35(LM35pin, true); // true = temp in celcius, false = temp in fahrenheit
  
  // print the results to the serial monitor:
  Serial.print(" Temp = " );                       
  Serial.println(LM35tempC);      

  //  Send status once every 30s
  delay(30 * 1000);                     
}

float readLM35(byte LM35Pin, boolean celcius){       
  int analogVal = 0;
  for(int i = 0; i < 10; i++) { // takes 10 samples to make sure we get a good value
    analogVal += (analogRead(LM35pin) - analogRead(LM35ref)); // subtract Vout ADC reading from LM35 ground ADC reading
    delay(10);
  }
  float tempC = (5.0 * analogVal * 10) / 1023;
  if (celcius == true) {
    return tempC; // return temperature in degrees Celcius
  } 
  else {
    return (tempC * 9 / 5) + 32; // return temperature in degrees Fahrenheit
  }
}

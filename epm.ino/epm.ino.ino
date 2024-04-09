#include "Arduino.h"
#include "Wire.h"
#include "DFRobot_VL53L0X.h"

// things to record
// time spent in open arms
// latency to middle
// time freezing before crossing into middle
// time freezing (first 2 min)
// freezing before crossing/latency to middle = % time frozen 

// recording devices
// int distSensor1 = A0;
// int distSensor2 = A1;
// int distSensor3 = A2;
// int distSensor4 = A3;
int xShutTop = 8;
int xShutRight = 9;

// raw readings
float distReading1;
float distReading2;
float distReading3;
// float distReading4;

// converted readings
float distCm1;
float distCm2;
float distCm3;
// float distCm4;

// converted arrays for averaging
const int nAvg = 10;
int avgIndex = 0;
float average1[nAvg];
float average2[nAvg];
float average3[nAvg];
// float[nAvg] average4;

int max_range = 520;
double adc_solution = 1023.0;

//sensor 3
DFRobot_VL53L0X sensor;
DFRobot_VL53L0X sensorRight;

float leftInit;
float leftSum = 0;
float leftI = 0;
float rightInit;
float rightSum = 0;
float rightI = 0;
float topInit;
float topSum = 0;
float topI = 0;

void setup() {
  Serial.begin(9600);

  pinMode(xShutTop, OUTPUT);
  digitalWrite(xShutTop, LOW);
  pinMode(xShutRight, OUTPUT);
  digitalWrite(xShutRight, LOW);
  delay(10);
  digitalWrite(xShutTop, HIGH);
  pinMode(xShutTop, INPUT);

  Wire.begin(); // Initialize I2C communication

  // Set I2C sub-device address for sensor
  sensor.begin();
  // sensorRight.begin(0x51);
  sensor.setMode(sensor.eContinuous, sensor.eHigh);
  sensor.start();

  digitalWrite(xShutRight, HIGH);
  pinMode(xShutRight, INPUT);

  sensorRight.begin(0x30);
  // sensorRight.begin(0x51);
  sensorRight.setMode(sensor.eContinuous, sensor.eHigh);
  sensorRight.start();

  // // Set I2C sub-device address for sensorRight
  // sensorRight.begin(0x51);
  // sensorRight.setMode(sensor.eContinuous, sensor.eHigh);
  // sensorRight.start();

  // Turn off all ports for both devices
  // Wire.beginTransmission(0x50); // device 1
  // Wire.write(0x00); // all ports off
  // Wire.endTransmission();

  // Wire.beginTransmission(0x51); // device 2
  // Wire.write(0x00); // all ports off
  // Wire.endTransmission();
  // sensor.setAddress(0x2A + 1);
  // sensors.startContinuous(50);
  // for sensor3
  //join i2c bus (address optional for master)
  // Wire.begin();
  // //Set I2C sub-device address
  // sensor.begin(0x50);
  // //Set to Back-to-back mode and high precision mode
  // sensor.setMode(sensor.eContinuous,sensor.eHigh);
  // //Laser rangefinder begins to work
  // sensor.start();


  // //Set I2C sub-device address
  // sensorRight.begin(0x51);
  // //Set to Back-to-back mode and high precision mode
  // sensorRight.setMode(sensor.eContinuous,sensor.eHigh);
  // //Laser rangefinder begins to work
  // sensorRight.start();

  // Wire.begin();
  // Wire.beginTransmission(0x50); // device 1
  // Wire.write(0x00); // all ports off
  // Wire.endTransmission();
  // Wire.begin();
  // Wire.beginTransmission(0x51); // device 2
  // Wire.write(0x00); // all ports off
  // Wire.endTransmission();
}

void loop() {
  // read the value from the sensor:
  distReading1 = sensorRight.getDistance();
  // distReading2 = analogRead(distSensor2);
  distReading3 = sensor.getDistance();
  // distReading4 = analogRead(distSensor4);

  // account for range vs 10 bit accuracy
  distCm1 = (distReading1 - 25) / 10;
  // distCm2 = distReading2 * max_range / adc_solution;
  distCm3 = (distReading3 - 25) / 10;
  // distCm2 = distReading4 * max_range / adc_solution;

  // calibration
  // if (millis() < 5000){
  //   rightSum += distCm1;
  //   rightI ++;
  //   leftSum += distCm2;
  //   leftI ++;
  //   topSum += distCm3;
  //   topI ++;
  //   Serial.println("waiting");
  // } else if (millis() > 5000 && millis() < 6000) {
  //   rightInit = rightSum/rightI;
  //   leftInit = leftSum/leftI;
  //   topInit = topSum/topI;
  //   Serial.print("right ");
  //   Serial.print(rightInit);
  //   Serial.print("cm left ");
  //   Serial.print(leftInit);
  //   Serial.print("cm top ");
  //   Serial.print(topInit);
  //   Serial.println("cm");
  // } else {
  //   Serial.print("right ");
  //   Serial.print(distCm1, 0);
  //   Serial.print("cm left ");
  //   Serial.print(distCm2, 0);
  //   Serial.print("cm top ");
  //   Serial.print(distCm3, 0);
  //   Serial.println("cm");
  // }
  Serial.print("right ");
  Serial.print(distCm1, 0);
  // Serial.print("cm left ");
  // Serial.print(distCm2, 0);
  Serial.print(" cm top ");
  Serial.print(distCm3, 0);
  Serial.println("cm");
  delay(100);
}

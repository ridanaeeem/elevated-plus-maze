#include "Arduino.h"
#include "Wire.h"
#include "DFRobot_VL53L0X.h"

// recording devices
int distSensor1 = A0;
int distSensor2 = A1;
// int distSensor3 = A2;
// int distSensor4 = A3;

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
  // for sensor3
  //join i2c bus (address optional for master)
  Wire.begin();
  //Set I2C sub-device address
  sensor.begin(0x50);
  //Set to Back-to-back mode and high precision mode
  sensor.setMode(sensor.eContinuous,sensor.eHigh);
  //Laser rangefinder begins to work
  sensor.start();
}

void loop() {
  // read the value from the sensor:
  distReading1 = analogRead(distSensor1);
  distReading2 = analogRead(distSensor2);
  distReading3 = sensor.getDistance();
  // distReading4 = analogRead(distSensor4);

  // account for range vs 10 bit accuracy
  distCm1 = distReading1 * max_range / adc_solution;
  distCm2 = distReading2 * max_range / adc_solution;
  distCm3 = (distReading3 - 25) / 10;
  // distCm2 = distReading4 * max_range / adc_solution;

  // add to array for averaging
  // float avg1 = 0;
  // float avg2 = 0;
  // if (avgIndex < nAvg) {
  //   avgIndex++;
  //   // average the array
  //   float tot1 = 0;
  //   float tot2 = 0;
  //   for (int i = 0; i < nAvg; i++) {
  //     tot1 += average1[i];
  //     tot2 += average1[i];
  //   }
  //   avg1 = tot1 / nAvg;
  //   avg2 = tot2 / nAvg;
  // } else avgIndex = 0;
  // average1[avgIndex] = distCm1;
  // average2[avgIndex] = distCm2;
  // average3[avgIndex] = distCm3;
  // average4[avgIndex] = distCm4;

  // print results
  // Serial.print(avg1, 0);
  // Serial.print("cm ");
  // Serial.print(avg2, 0);
  // Serial.println("cm");

  // calibration
  if (millis() < 5000){
    rightSum += distCm1;
    rightI ++;
    leftSum += distCm2;
    leftI ++;
    topSum += distCm3;
    topI ++;
    Serial.println("waiting");
  } else if (millis() > 5000 && millis() < 6000) {
    rightInit = rightSum/rightI;
    leftInit = leftSum/leftI;
    topInit = topSum/topI;
    Serial.println(rightInit);
    Serial.println(leftInit);
    Serial.println(topInit);
  } else {
    Serial.print("right ");
    Serial.print(distCm1, 0);
    Serial.print("cm ");
    Serial.print("left ");
    Serial.print(distCm2, 0);
    Serial.print("cm ");
    Serial.print("top ");
    Serial.print(distCm3, 0);
    Serial.println("cm");
  }
  delay(100);
}

#include "Arduino.h"
#include "Wire.h"
#include "DFRobot_VL53L0X.h"



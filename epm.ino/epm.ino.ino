#include "Adafruit_VL53L0X.h"

// address we will assign if sensor is present
#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31
#define LOX3_ADDRESS 0x32
#define LOX4_ADDRESS 0x34

// set the pins to shutdown
#define SHT_LOX1 9
#define SHT_LOX2 10
#define SHT_LOX3 11
#define SHT_LOX4 12

// objects for the vl53l0x
Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox3 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox4 = Adafruit_VL53L0X();

// this holds the measurement
VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;
VL53L0X_RangingMeasurementData_t measure3;
VL53L0X_RangingMeasurementData_t measure4;

//right, left, bottom, top
double prev[] = {0,0,0,0};
double curr[] = {0,0,0,0};

bool frozen = false;

// each sensor is 30cm away from the one across it
double mouseLength = 8;
double mousewidth = 1;

double xPosition;
double yPosition;
double prevX;
double prevY;

double lastTime;
double timeFrozen = 0;
bool updateTime = false;
double lastTimeOpenArms;
double timeFrozenOpenArms = 0;
bool updateTimeOpenArms = false; 

/*
    Reset all sensors by setting all of their XSHUT pins low for delay(10), then set all XSHUT high to bring out of reset
    Keep sensor #1 awake by keeping XSHUT pin high
    Put all other sensors into shutdown by pulling XSHUT pins low
    Initialize sensor #1 with lox.begin(new_i2c_address) Pick any number but 0x29 and it must be under 0x7F. Going with 0x30 to 0x3F is probably OK.
    Keep sensor #1 awake, and now bring sensor #2 out of reset by setting its XSHUT pin high.
    Initialize sensor #2 with lox.begin(new_i2c_address) Pick any number but 0x29 and whatever you set the first sensor to
 */

void setup() {
  Serial.begin(115200);

  // set all shutdown pins as outputs
  pinMode(SHT_LOX1, OUTPUT);
  pinMode(SHT_LOX2, OUTPUT);
  pinMode(SHT_LOX3, OUTPUT);
  pinMode(SHT_LOX4, OUTPUT);

  Serial.println(F("Shutdown pins inited..."));

  // all reset
  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  digitalWrite(SHT_LOX4, LOW);

  Serial.println(F("All in reset mode...(pins are low)"));
  
  Serial.println(F("Starting..."));
  

  delay(10);
  // all unreset
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, HIGH);
  digitalWrite(SHT_LOX3, HIGH);
  digitalWrite(SHT_LOX4, HIGH);
  delay(10);

  // activating LOX1 and resetting others
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  digitalWrite(SHT_LOX4, LOW);

  // initing LOX1
  if(!lox1.begin(LOX1_ADDRESS)) {
    Serial.println(F("Failed to boot first VL53L0X"));
    while(1);
  }
  delay(10);

  // activating LOX2
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  //initing LOX2
  if(!lox2.begin(LOX2_ADDRESS)) {
    Serial.println(F("Failed to boot second VL53L0X"));
    while(1);
  }
  delay(10);
 
  // activating LOX3
  digitalWrite(SHT_LOX3, HIGH);
  delay(10);

  //initing LOX3
  if(!lox3.begin(LOX3_ADDRESS)) {
    Serial.println(F("Failed to boot third VL53L0X"));
    while(1);
  }
  delay(10);
 
  // activating LOX4
  digitalWrite(SHT_LOX4, HIGH);
  delay(10);

  //initing LOX4
  if(!lox4.begin(LOX4_ADDRESS)) {
    Serial.println(F("Failed to boot third VL53L0X"));
    while(1);
  }

  lox1.configSensor(Adafruit_VL53L0X::VL53L0X_SENSE_DEFAULT);
  lox2.configSensor(Adafruit_VL53L0X::VL53L0X_SENSE_DEFAULT);
  lox3.configSensor(Adafruit_VL53L0X::VL53L0X_SENSE_DEFAULT);
  lox4.configSensor(Adafruit_VL53L0X::VL53L0X_SENSE_DEFAULT);
  /* Set desired configurations from the options below:
   *    VL53L0X_SENSE_DEFAULT
        VL53L0X_SENSE_LONG_RANGE
        VL53L0X_SENSE_HIGH_SPEED
        VL53L0X_SENSE_HIGH_ACCURACY
   */
}

void loop() {  
  if (frozen) {
    // started freezing
    if (updateTime){
      lastTime = millis();
      lastTimeOpenArms = millis(); 
      updateTime = false;
    } 
    Serial.println("FROZEN");
  }
  else {
    Serial.println("NOT FROZEN");
    // update time if haven't already
    if (!updateTime){
      timeFrozen += (millis() - lastTime);
      if (yPosition < 0 || yPosition > 35){
        timeFrozenOpenArms += (millis() - lastTimeOpenArms);
      }
      updateTime = true;
    }
  }
  // Serial.println("right, left, bottom, top");
  // for (int i=0; i<4; i++){
  //   Serial.print(prev[i]);
  //   Serial.print(" ");
  // }
  // Serial.println();
  // for (int i=0; i<4; i++){
  //   Serial.print(curr[i]);
  //   Serial.print(" ");
  // }
  Serial.println();
  Serial.print("Total time (ms): ");
  Serial.print(millis());
  Serial.println();
  Serial.print("Total time frozen (ms): ");
  Serial.print(timeFrozen);
  Serial.println();
  Serial.print("Total time frozen in open arms (ms): ");
  Serial.print(timeFrozenOpenArms);
  Serial.println();
  double measure1Mm = measure1.RangeMilliMeter;
  double measure2Mm = measure2.RangeMilliMeter;
  double measure3Mm = measure3.RangeMilliMeter;
  double measure4Mm = measure4.RangeMilliMeter;

  // adjusted mm values 
  double measure1CmAdjusted = measure1Mm / 10;
  double measure2CmAdjusted = measure2Mm / 10;
  double measure3CmAdjusted = measure3Mm / 10;
  double measure4CmAdjusted = measure4Mm / 10;

  // first iteration, set current measurements
  if (curr[0] == 0){
    curr[0] = measure1CmAdjusted;
    curr[1] = measure2CmAdjusted;
    curr[2] = measure3CmAdjusted;
    curr[3] = measure4CmAdjusted;
    
  // second iteration, set those last current to previous and update current
  // same for every future iteration
  } else {
    // prev = curr;
    for (int i=0; i<4; i++){
      prev[i] = curr[i];
    } 
    // curr = new
    curr[0] = measure1CmAdjusted;
    curr[1] = measure2CmAdjusted;
    curr[2] = measure3CmAdjusted;
    curr[3] = measure4CmAdjusted;
  }

  // read_dual_sensors();
  lox1.rangingTest(&measure1, false); // pass in 'true' to get debug data printout!
  lox2.rangingTest(&measure2, false); // pass in 'true' to get debug data printout!
  lox3.rangingTest(&measure3, false); // pass in 'true' to get debug data printout!
  lox4.rangingTest(&measure4, false); // pass in 'true' to get debug data printout!

  prevX = xPosition;
  prevY = yPosition;

  // originally tried calibrating and getting an absolute positiom
  // left - (30 - left - right)
  // double diff = 30 - curr[0] - curr[1];
  // xPosition = curr[1] + diff;
  // right 
  // diff = 30 - curr[2] - curr[3];
  // yPosition = curr[2] + diff;

  // x = left, y = bottom worked better
  xPosition = curr[1];
  yPosition = curr[2];

  // calculating freezing
  // double threshold = 0.2;
  // default
  double threshold = 0.4;

  double deltaLeft = abs(prev[0] - curr[0]);
  double deltaRight = abs(prev[1] - curr[1]);
  double minX = min(deltaLeft, deltaRight);
  double maxX = max(deltaLeft, deltaRight);
  double deltaBottom = abs(prev[2] - curr[2]);
  double deltaTop = abs(prev[3] - curr[3]);
  double minY = min(deltaBottom, deltaTop);
  double maxY = max(deltaBottom, deltaTop);

  //   left and right example:
  //      Δleft and Δright both >= threshold
  //      Δmax <= Δmin * 1.5 (should have pretty similar changes in one axis)
  if (deltaLeft >= threshold && deltaRight >= threshold && xPosition > 0
  && maxX <= minX * 1.5) {
    frozen = false;
  } else if (deltaBottom >= threshold && deltaTop >= threshold && yPosition > 0
  && maxY <= minY * 1.5) {
    frozen = false;
  } else {
    frozen = true;
  }

  // coordinate printing
  Serial.println("XPOSITION + YPOSITION ");
  Serial.print(xPosition);
  Serial.print(" ");
  Serial.print(yPosition);
  Serial.println();


  // extra printing for testing

  // Serial.println("right, left, bottom, top");
  // // print sensor one reading
  // // Serial.print(F("1: "));
  // if(measure1.RangeStatus != 4) {     // if not out of range
  //   Serial.print(measure1CmAdjusted);
  // } else {
  //   Serial.print(F("Out of range"));
  // }
  
  // Serial.print(F(", "));

  // // print sensor two reading
  // // Serial.print(F("2: "));
  // if(measure2.RangeStatus != 4) {
  //   Serial.print(measure2CmAdjusted);
  // } else {
  //   Serial.print(F("Out of range"));
  // }
  
  // Serial.print(F(", "));

  // // print sensor three reading
  // // Serial.print(F("3: "));
  // if(measure3.RangeStatus != 4) {
  //   Serial.print(measure3CmAdjusted);
  // } else {
  //   Serial.print(F("Out of range"));
  // }

  // Serial.print(F(", "));

  // // print sensor four reading
  // // Serial.print(F("4: "));
  // if(measure4.RangeStatus != 4) {
  //   Serial.print(measure4CmAdjusted);
  // } else {
  //   Serial.print(F("Out of range"));
  // }
  // Serial.println();

  // for (int i=0; i<4; i++){
  //   Serial.print(prev[i]);
  //   Serial.print(" ");
  // }
  // Serial.println();
  // for (int i=0; i<4; i++){
  //   Serial.print(curr[i]);
  //   Serial.print(" ");
  // }
  // Serial.println();
}

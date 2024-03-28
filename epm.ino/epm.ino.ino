// recording devices
int distSensor1 = A0; 
int distSensor2 = A1;
int distSensor3 = A2;
int distSensor4 = A3;

int max_range = 520;
double adc_solution = 1023.0;

float distReading1;
float distReading2;
float distReading3;
float distReading4;

float distCm1;
float distCm2;
float distCm3;
float distCm4;

void setup() {
  Serial.begin(9600);
}

void loop() {
  // read the value from the sensor:
  distReading1 = analogRead(distSensor1);
  distReading2 = analogRead(distSensor2);
  // distReading3 = analogRead(distSensor3);
  // distReading4 = analogRead(distSensor4);

  // account for range vs 10 bit accuracy
  distCm1 = distReading1 * max_range / adc_solution;  
  distCm2 = distReading2 * max_range / adc_solution;  
  // distCm1 = distReading3 * max_range / adc_solution;  
  // distCm2 = distReading4 * max_range / adc_solution;  

  // print results
  Serial.print(distCm1, 0);
  Serial.print("cm ");
  Serial.print(distCm2, 0);
  Serial.println("cm");
  // Serial.print(distCm3, 0);
  // Serial.print("cm ");
  // Serial.print(distCm4, 0);
  // Serial.println("cm");

  delay(100);
}


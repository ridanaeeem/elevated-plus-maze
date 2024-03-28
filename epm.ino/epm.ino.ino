// recording devices
int distSensor = A0; 
int distSensor2 = A1;

int max_rang = 520;
double adc_solution = 1023.0;

float distReading;
float distReading2;
float dist_t;
float dist_t2;

void setup() {
  Serial.begin(9600);
}

void loop() {
  // read the value from the sensor:
  distReading = analogRead(distSensor);
  distReading2 = analogRead(distSensor2);
  dist_t = distReading * max_rang / adc_solution;  //
  dist_t2 = distReading2 * max_rang / adc_solution;  //
  Serial.print(dist_t, 0);
  Serial.print("cm ");
  Serial.print(dist_t2, 0);
  Serial.println("cm");

  delay(100);
}


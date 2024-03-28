// recording devices
int distSensor = A0; 
// recording values
float sensorReading;

void setup() {
  Serial.begin(9600);
}

void loop() {
  sensorReading = analogRead(distSensor);
  Serial.print(sensorReading, 0);
  Serial.println("cm");

  delay(100);
}

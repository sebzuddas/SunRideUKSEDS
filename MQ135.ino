int sensorPin = A0;
 
void setup() {
Serial.begin(9600);
 
pinMode(sensorPin, INPUT);
}
 
void loop() {  
  int reading = analogRead(SensorPin); 
  Serial.print("Gas sensor reading: ");
  Serial.println(reading);
  delay(1000);
}

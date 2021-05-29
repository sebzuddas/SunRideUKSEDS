  
int sensorPin = A0;
 
void setup() {
Serial1.begin(9600);
Serial.begin(9600);
 
pinMode(sensorPin, INPUT);
}
 
void loop() {  

  for(int i =0;i<100;i++){
  int reading = analogRead(sensorPin); 
  Serial1.print(reading);
  Serial.print(reading);
  delay(10);
  i++;
  }

 // while(1){};
}

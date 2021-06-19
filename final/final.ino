/*
 * Project Sunride's UKSEDS 2021 team Avionics .
 * An Arduino Nano 33 BLE Sense is used to record temperature, pressure, air quality, and the velocity and acceleration of the rocket itself.
 * This data is then stored on an SD Card using the adafruit MicroSD Breakout board.
 * The pinout of the BLE Sense can be found here: https://content.arduino.cc/assets/Pinout-NANOsense_latest.pdf
 * A link to a tutorial for the SD card board can be found here: https://learn.adafruit.com/adafruit-micro-sd-breakout-board-card-tutorial?view=all
 * Created by :
 * Date: 2021
*/
//////////////////////////////////////////////////////////////
//libraries
#include <Arduino_LSM9DS1.h>// Library for gyroscope
#include <Arduino_LPS22HB.h>// Library for barometer
#include <Arduino_HTS221.h>//Library for the temperature and humidity
#include <SPI.h>
#include <SdFat.h>//Same as SD.h but allows longer file names
///////////////////////////////////////////////
//definitions
#define baseName "LaunchData"
#define GPSSerial Serial1
#define sensorPin A0
////////////////////////////////////////////////////////////
//variable declaration
const int CSPin = 4;
const int baseNameSize = sizeof(baseName) - 1;
char fileName[] = baseName "00.txt";
unsigned long t;
unsigned long Previous_time = 0;
float temperature;
float humidity;
char c;
////////////////////////////////////////////////////////
//create Sdfat object: SD
SdFat SD;

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
    // wait for hardware serial to appear
 
  GPSSerial.begin(9600);

  if (!SD.begin(CSPin)) {// check if it initializes. 
    Serial.println("Initialization failed/card not present");
    while (1){
      error(2);
    }
  }
  while (SD.exists(fileName)) {//create the first file
    if (fileName[baseNameSize + 1] != '9') {
      fileName[baseNameSize + 1]++;
    } else if (fileName[baseNameSize] != '9') {
      fileName[baseNameSize + 1] = '0';
      fileName[baseNameSize]++;
    } else {
      Serial.println("Can't create file name");
      error(3);
      return;
    }
  }
  
  Serial.println("Card initialized");
  Serial.print("Created: ");
  Serial.println(fileName);
  Serial.println("");
}
 
void loop() {

  
  String data = "";// variable to put the string data in 
  //Add the other data here and add to data string



  float x, y, z;
  // read the sensor values and put them into a float variable
    float pressure = BARO.readPressure();
    float temperature = HTS.readTemperature();
    float humidity = HTS.readHumidity();
  //if the accelerometer is getting a reading, then read, put into variables and print the variables
    if (IMU.accelerationAvailable()) {
      IMU.readAcceleration(x, y, z);
      Serial.print(x);
      Serial.print('\t');
      Serial.print(y);
      Serial.print('\t');
      Serial.println(z);
    }
    //Detect launch with acceleration  !!!!!!!Don't know if this should go in void loop!!!!!
    // accelMag = sqrt(x^2 + y^2 + z^2)//Magnitude of the acceleration
    //if (accelMag > 5) {//If the acceleration is higher than 5gs then start the recording of the data.
    //start recording
    //need to make it so that the acceleration is high for like 500ms before recording starts
    //Would it be possible to also save the data for the 20 or so seconds before the launch?
    //}
  
  //C02 and N02 Sensor, Ashley's code for MQ135
    int gasReading = analogRead(sensorPin); //gas sensor reading

  t = millis();// function to store time since device was reset
  data += String(t) + ", " ;
 
  if (millis() - Previous_time >= 30000) {// create a new file every 30 seconds
    Previous_time = millis();
    while (SD.exists(fileName)) {
    if (fileName[baseNameSize + 1] != '9') {
      fileName[baseNameSize + 1]++;
    } else if (fileName[baseNameSize] != '9') {
      fileName[baseNameSize + 1] = '0';
      fileName[baseNameSize]++;
    } else {
      Serial.println("Can't create file name");//throw error if file can't be created.
      error(4);
      return;
    }
  }
}

  if (Serial.available()) {
    char c = Serial.read();
    GPSSerial.write(c);
  }
  if (GPSSerial.available()) {
    char c = GPSSerial.read();
    Serial.write(c);
  }
  data += String(t) + ", " + String(temperature) + ", " + String(humidity)+ ", " + String(pressure)+", " + String(gasReading)+", "+c;
  File dataFile = SD.open(fileName, FILE_WRITE);// open the file to write to it
  if (dataFile) {
    dataFile.println(data);//print the data variable onto the file
    dataFile.close();// close the file
    //Serial.println(data);
  }
  else {
    //Serial.print("Error opening ");
    //Serial.println(fileName);
    error(5);
  }
}

// function that flashes onboard led a times so that we can detect where and what the error is.
void error(int a){
  for(int j=1;j<a;j++){
  digitalWrite(LED_BUILTIN,HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN,LOW);
  delay(500);  
  j++;
  }
}

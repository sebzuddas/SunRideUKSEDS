#include <ArduinoBLE.h>

#include <SPI.h>
#include <SdFat.h>
#include <Arduino_LSM9DS1.h>// Library for gyroscope
#include <Arduino_LPS22HB.h>// Library for barometer
#include <Arduino_HTS221.h>//Library for the temperature and humidity
SdFat SD;

#define baseName "LaunchData"

const int CSPin = 4;

const int baseNameSize = sizeof(baseName) - 1;
char fileName[] = baseName "00.txt";

//#include <DHT.h>
#define Type DHT11

int sensorPin = A0;
float temperature;
float humidity;

//DHT HT(sensorPin,Type);

unsigned long t;
 
void setup() {
  HT.begin();
  Serial.begin(9600);
 
  Serial.println("Initializing SD card");
 
  if (!SD.begin(CSPin)) {
    Serial.println("Initialization failed/card not present");
    while (1);
  }

 
  
  Serial.println("Card initialized");
  Serial.print("Created: ");
  Serial.println(fileName);
  Serial.println("");

  pinMode(sensorPin, INPUT);
 
 
  //make sure that sensor values are zero-ed
//initialise sensors on ble 
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  if (!BARO.begin()) {
    Serial.println("Failed to initialize pressure sensor!");
    while (1);
  }
  if (!HTS.begin()) {
    Serial.println("Failed to initialize humidity temperature sensor!");
    while (1);
  }
// gives the sample rate of the accelerometer.
  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Acceleration in G's");
  Serial.println("X\tY\tZ");
}


 
void loop() {
  String data = "";
  
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


t = millis();
for (int i=0;i<30;i++){
   while (SD.exists(fileName)) {
      if (fileName[baseNameSize + 1] != '9') {
        fileName[baseNameSize + 1]++;
      } else if (fileName[baseNameSize] != '9') {
        fileName[baseNameSize + 1] = '0';
        fileName[baseNameSize]++;
      } else {
        Serial.println("Can't create file name");
        return;
      }
    }
    while(t<){

    data += String(t) + ", " + String(temperature) + ", " + String(humidity)+ ", " + String(pressure)+", " + String(gasSensor);
    File dataFile = SD.open(fileName, FILE_WRITE);
    if (dataFile) {
      dataFile.println(data);
      dataFile.close();
    }
    else {
      Serial.print("Error opening ");
      Serial.println(fileName);
    }
    delay(10); // sampling rate BIATCH 
  }
}

  i++;
}

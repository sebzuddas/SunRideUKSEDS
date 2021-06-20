#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h>// Library for gyroscope
#include <Arduino_LPS22HB.h>// Library for barometer
#include <Arduino_HTS221.h>//Library for the temperature and humidity

#include <SPI.h>
#include <SdFat.h>
SdFat SD;

#define baseName "LaunchData"

const int CSPin = 4;

const int baseNameSize = sizeof(baseName) - 1;
char fileName[] = baseName "00.txt";

float pressureAtSea = 101325;

int sensorPin = A0;

unsigned long t;
unsigned long Previous_time = 0;
 
void setup() {
  Serial.begin(9600);
 
  Serial.println("Initializing SD card");
 
  if (!SD.begin(CSPin)) {
    Serial.println("Initialization failed/card not present");
    while (1);
  }

//  while (SD.exists(fileName)) {
//    if (fileName[baseNameSize + 1] != '9') {
//      fileName[baseNameSize + 1]++;
//    } else if (fileName[baseNameSize] != '9') {
//      fileName[baseNameSize + 1] = '0';
//      fileName[baseNameSize]++;
//    } else {
//      Serial.println("Can't create file name");
//      return;
//    }
//  }

  Serial.println("Card initialized");
  Serial.print("Created: ");
  Serial.println(fileName);
  Serial.println("");

  pinMode(sensorPin, INPUT);//For the gas sensor
 
 
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
  // gives the sample rate of the accelerometer and gyroscope.
  //Serial.print("Accelerometer sample rate = ");
  //Serial.print(IMU.accelerationSampleRate());
  //Serial.println(" Hz");
  //Serial.println();
  //Serial.println("Acceleration in G's");
  //Serial.println("X\tY\tZ");
  //
  //Serial.print("Gyroscope sample rate = ");
  //Serial.print(IMU.gyroscopeSampleRate());
  //Serial.println(" Hz");
  //Serial.println();
  //Serial.println("Gyroscope in degrees/second");
  //Serial.println("X\tY\tZ");
}

void loop() {
  String data = "";
  
  // read the sensor values and put them into a float variable
  float pressure = BARO.readPressure() * 1000;
  float temperature = HTS.readTemperature();
  float humidity = HTS.readHumidity();

  //Gas sensor reading
  int gasReading = analogRead(sensorPin);

  
  //if the accelerometer is getting a reading, then read and put into variables
  float ax, ay, az;
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(ax, ay, az);
  }
  
  //if the gyroscope is getting a reading, then read and put into variables
  float gx, gy, gz;
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(gx, gy, gz);
  }

  //Altitude calculation
  float H = ((pow((pressureAtSea/pressure), (1/5.257))-1)*(15 + 273.15))/(0.0065);
  
  //Detect launch with acceleration  !!!!!!!Don't know if this should go in void loop!!!!!
  // accelMag = sqrt(ax^2 + ay^2 + az^2)//Magnitude of the acceleration
  //if (accelMag > 5) {//If the acceleration is higher than 5gs then start the recording of the data.
  //start recording
  //need to make it so that the acceleration is high for like 500ms before recording starts
  //Would it be possible to also save the data for the 20 or so seconds before the launch?
  //}

  t = millis();
  
  if (millis() - Previous_time >= 30000) {
    Previous_time = millis();
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
  }

  data += String(t) + ", " + String(ax) + ", " + String(ay)+ ", " + String(az)+ ", " + String(gx)+ ", " + String(gy)+ ", " + String(gz)+ ", " + String(pressure)+ ", " + String(temperature)+ ", " + String(humidity)+ ", " + String(H)+ ", " + String(gasReading);
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

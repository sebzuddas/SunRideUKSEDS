/* Project Sunride's UKSEDS 2021 team Avionics.
 * An Arduino Nano 33 BLE Sense is used to record acceleration, rate of rotation, air pressure, temperature, humidity and altitude of the rocket itself.
 * This data is then stored on an SD Card using the adafruit MicroSD Breakout board.
 * The pinout of the BLE Sense can be found here: https://content.arduino.cc/assets/Pinout-NANOsense_latest.pdf
 * A link to a tutorial for the SD card board can be found here: https://learn.adafruit.com/adafruit-micro-sd-breakout-board-card-tutorial?view=all
 * Created by :
/* Date: 2021  
 */
//////////////////////////////////////////////////////////////
//Libraries
#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h> // Library for Accelerometer and gyroscope
#include <Arduino_LPS22HB.h> // Library for barometer, 
#include <Arduino_HTS221.h> //Library for the temperature and humidity

#include <SPI.h> //SPI labrary for micro SD card reader
#include <SdFat.h> ////SD card reader library, same as SD.h but allows longer file names

#include <Adafruit_GPS.h> //Adafruit GPS library
///////////////////////////////////////////////
//Definitions
#define baseName "LaunchData"
#define GPSSerial Serial1
#define GPSECHO false
#define CSPin 4
#define sensorPin A0
////////////////////////////////////////////////////////////
//Variable Declaration
const int baseNameSize = sizeof(baseName) - 1;
char fileName[] = baseName "00.txt";

float pressureAtSea = 101325;

unsigned long t;

unsigned long Previous_time = 0;
///////////////////////////////////////////////////////////
SdFat SD;
Adafruit_GPS GPS(&GPSSerial);

void setup() {
  Serial.begin(115200);
  GPS.begin(9600);

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

  pinMode(sensorPin, INPUT); //For the gas sensor

  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PGCMD_ANTENNA);
 
  //Initialise sensors on ble 
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
}

void loop() {
  String data = ""; //Variable to put the string data in 
  
  //Read the sensor values and put them into a float variable
  float pressure = BARO.readPressure() * 1000;
  float temperature = HTS.readTemperature();
  float humidity = HTS.readHumidity();

  //Gas sensor reading
  int gasReading = analogRead(sensorPin);

  
  //If the accelerometer is getting a reading, then read and put into variables
  float ax, ay, az;
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(ax, ay, az);
  }
  
  //If the gyroscope is getting a reading, then read and put into variables
  float gx, gy, gz;
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(gx, gy, gz);
  }

  //Altitude calculation
  float height = ((pow((pressureAtSea/pressure), (1/5.257))-1)*(15 + 273.15))/(0.0065);
  
  //Detect launch with acceleration  !!!!!!!Don't know if this should go in void loop!!!!!
  // accelMag = sqrt(ax^2 + ay^2 + az^2)//Magnitude of the acceleration
  //if (accelMag > 5) {//If the acceleration is higher than 5gs then start the recording of the data.
  //start recording
  //need to make it so that the acceleration is high for like 500ms before recording starts
  //Would it be possible to also save the data for the 20 or so seconds before the launch?
  //}

  //GPS stuff
  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA()))
      return;
  }

  int fixed = (int)GPS.fix;
  int quality = (int)GPS.fixquality;

  float lat1;
  char lat2;
  float long1;
  char long2;
  float speed1;
  float altitude1;
  int sats;

  if (GPS.fix == 1) {
    lat1 = (GPS.latitude);
    lat2 = (GPS.lat);
    long1 = (GPS.longitude);
    long2 = (GPS.lon);
    speed1 = (GPS.speed);
    altitude1 = (GPS.altitude);
    sats = ((int)GPS.satellites);
  }

  else{
    lat1 = 0;
    lat2 = 'x';
    long1 = 0;
    long2 = 'x';
    speed1 = 0;
    altitude1 = 0;
    sats = 0;
  }

  //Function to store time since device was powered on/reset
  t = millis();

  //Create new file every 30 seconds
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

  //Writing to the micro SD card
  data += String(t) + ", " + String(ax) + ", " + String(ay) + ", " + String(az) + ", " + String(gx) + ", " + String(gy) + ", " + String(gz) + ", " + String(pressure) + ", " + String(temperature)
  + ", " + String(humidity) + ", " + String(height) + ", " + String(gasReading) + ", " + String(fixed) + ", " + String(quality) + ", " + String(lat1) + String(lat2) + ", " + String(long1) + String(long2)
  + ", " + String(speed1) + ", " + String(altitude1) + ", " + String(sats);
  
  File dataFile = SD.open(fileName, FILE_WRITE);
  if (dataFile) {
    dataFile.println(data);
    dataFile.close();
  }
  else {
    Serial.print("Error opening ");
    Serial.println(fileName);
  }
  //delay(10); //Sampling rate BIATCH 
}

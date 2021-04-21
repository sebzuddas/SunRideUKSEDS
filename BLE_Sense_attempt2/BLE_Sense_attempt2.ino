/*
 * Project Sunride's UKSEDS 2021 team Avionics .
 * An Arduino Nano 33 BLE Sense is used to record temperature, pressure, air quality, and the velocity and acceleration of the rocket itself.
 * This data is then stored on an SD Card using the adafruit MicroSD Breakout board.
 * The pinout of the BLE Sense can be found here: https://content.arduino.cc/assets/Pinout-NANOsense_latest.pdf
 * A link to a tutorial for the SD card board can be found here: https://learn.adafruit.com/adafruit-micro-sd-breakout-board-card-tutorial?view=all
 * Created by :
 * Date: 2021
*/

#include <Arduino_LSM9DS1.h>// Library for gyroscope
#include <Arduino_LPS22HB.h>// Library for barometer
#include <Arduino_HTS221.h>//Library for the temperature and humidity
 
  
const  int sensorPin = A0;

// ATTENTION add time function to present the timestamp

//ATTENTION write GPS code (found on separate file)

// ATTENTION make program for launch/recovery detection (based on IMU and timestamp)

void setup() {
 // ATTENTION please add commands to set the pins as OUTPUT,INPUT,HIGH,LOW for every sensor in this system
 
 // ATTENTION check if values should be defined as int or const int (there's something about saving location in main memory or cache I think)
 
pinMode(sensorPin, INPUT);
 
  
  //make sure that sensor values are zero-ed
  //initialise serial comms
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Started");
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
  // declare float variables for the accelerometer
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
  
 //C02 and N02 Sensor, Ashley's code for MQ130
  int reading = analogRead(SensorPin); 
  Serial.print("Gas sensor reading: ");
  Serial.println(reading); //ATTENTION translate the output values  0 - 1048 to---->  actual percentages of air mixture. Use the mapping function to present them.
  delay(2);
 
//separate with new line
Serial.println();
//print Pressure sensor value
  Serial.print("Pressure = ");
  Serial.print(pressure);
  Serial.println(" kPa");
//separate
Serial.println();
//print temperature sensor values
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" °C");
//separate
Serial.println();
//print humidity sensor values
  Serial.print("Humidity    = ");
  Serial.print(humidity);
  Serial.println(" %");
//separate
Serial.println();
}

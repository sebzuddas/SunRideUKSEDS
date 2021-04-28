// this is the code branch for GPS location tracking to calculate the final recovery distance from the launchpad
//
//ATTENTION Jamiee: GPS traccking, application, main code
//Program function:
// Location
// Distance from Launchpad
// Estimated Altitude (Cedric/Seb)

#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup(){
  Serial.begin(9600);
  ss.begin(GPSBaud);
}

void loop(){
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0){
    gps.encode(ss.read());
    if (gps.location.isUpdated()){
      Serial.print("Latitude= "); 
      Serial.println(gps.location.lat(), 6);  // Latitude in degrees
      Serial.print(" Longitude= "); 
      Serial.println(gps.location.lng(), 6);  // Longitude in degrees
      Serial.print("Altitude= ")
      Serial.println(gps.altitude.meters()); // Altitude in meters

    }
  }
}

#include <SPI.h>
#include <SdFat.h>//Same as SD.h but allows longer file names
SdFat SD;

#define baseName "LaunchData"

const int CSPin = 4;

const int baseNameSize = sizeof(baseName) - 1;
char fileName[] = baseName "00.txt";

unsigned long t;
 
void setup() {
  HT.begin();
  Serial.begin(9600);
 
  Serial.println("Initializing SD card");
 
  if (!SD.begin(CSPin)) {
    Serial.println("Initialization failed/card not present");
    while (1);
  }

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
  
  Serial.println("Card initialized");
  Serial.print("Created: ");
  Serial.println(fileName);
  Serial.println("");
}
 
void loop() {
  String data = "";
  //Add the other data here and add to data string
  t = millis();
  data += String(t) + ", " ;
 
  File dataFile = SD.open(fileName, FILE_WRITE);
  if (dataFile) {
    dataFile.println(data);
    dataFile.close();
    Serial.println(data);
  }
  else {
    Serial.print("Error opening ");
    Serial.println(fileName);
  }
}

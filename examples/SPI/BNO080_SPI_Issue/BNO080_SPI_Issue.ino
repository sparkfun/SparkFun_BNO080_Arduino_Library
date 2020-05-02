// This example demonstrates what happens if you set the
// Rotation Vector Report Interval to 10ms but then
// deliberately only call dataAvailable() every 15ms
// to retrieve the SPI data

#include "SparkFun_BNO080_Arduino_Library.h"
BNO080 myIMU;

//These pins can be any GPIO
byte imuCSPin = 10;
byte imuWAKPin = 9;
byte imuINTPin = 8;
byte imuRSTPin = 7;

long measurements = 0;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("BNO080 SPI Read Example");
  Serial.println("Press any key to begin...");
  while (!Serial.available()) // Wait for a 'key press'
    ;
  while (Serial.available()) Serial.read(); // Clear the buffer
  
  myIMU.enableDebugging(); // Enable debug messages so we can use printPacket

  if(myIMU.beginSPI(imuCSPin, imuWAKPin, imuINTPin, imuRSTPin) == false)
  {
    Serial.println("BNO080 over SPI not detected. Are you sure you have all 7 connections? Freezing...");
    while(1);
  }

  myIMU.enableRotationVector(10); //Send data update every 10ms
}

void loop()
{
  delay(15); // Deliberately wait 15ms between calls to dataAvailable

  //Look for reports from the IMU
  if (myIMU.dataAvailable() == true)
  {
    measurements++;
  }

  // Pressing any key (sending one or more serial bytes)
  // prints the measurement tally
  if (Serial.available())
  {
    Serial.println();
    Serial.print(F("Measurements: "));
    Serial.println(measurements);
    while (Serial.available()) Serial.read();
  }
}

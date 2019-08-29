/*
  Using the BNO080 IMU
  By: Nathan Seidle
  SparkFun Electronics
  Date: August 29th, 2019
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14586

  This example shows how to detect taps

  Hardware Connections:
  Plug the sensor into the Qwiic connector on your RedBoard, Artemis, or other carrier board
  Serial.print it out at 9600 baud to serial monitor.
*/

#include <Wire.h>

#include "SparkFun_BNO080_Arduino_Library.h"
BNO080 myIMU;

int tapCount = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println();
  Serial.println("BNO080 Read Example");

  Wire.begin();

  myIMU.begin();

  Wire.setClock(400000); //Increase I2C data rate to 400kHz

  myIMU.enableTapDetector(50); //Send data update every 50ms

  Serial.println(F("Tap Detection enabled"));
}

void loop()
{
  //Look for reports from the IMU
  if (myIMU.dataAvailable() == true)
  {
    if(myIMU.getAndResetTapDetector() > 0)
    {
      Serial.print("Tap! ");
      Serial.println(tapCount++);
      
    }
  }
}

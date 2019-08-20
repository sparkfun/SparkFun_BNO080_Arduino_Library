/*
  Using the BNO080 IMU with helper methods
  By: Guillaume Walck

  Date: September 08th, 2020
  License: This code is public domain.

  This example shows how to output several data types using helper methods.

  Using SPI1 on Teensy4.0
*/


#include <SPI.h>
#include <Wire.h>

#include "SparkFun_BNO080_Arduino_Library.h"
BNO080 myIMU;

const byte imuMOSIPin = 26; 
const byte imuMISOPin = 1; 
const byte imuSCKPin = 27; 
const byte imuCSPin = 0; 
//Further IMU pins
const byte imuWAKPin = 24;  //PS0
const byte imuINTPin = 25;  //INT
const byte imuRSTPin = 2;  //RST

bool imu_initialized = false;
bool imu_calibrated = false;

// internal copies of the IMU data
float ax, ay, az, gx, gy, gz, mx, my, mz, qx, qy, qz, qw; // (qx, qy, qz, qw = to i,j,k, real)

bool timer_started = false;

void setup() {
  // set the Slave Select Pins as outputs:
  pinMode (imuCSPin, OUTPUT);  

  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  Serial.setTimeout(500); //timeout of 500 ms
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }

  // set up the SPI pins utilized on Teensy 4.0
  SPI1.setMOSI(imuMOSIPin);
  SPI1.setMISO(imuMISOPin);
  SPI1.setSCK(imuSCKPin);  
  // initialize SPI1:
  SPI1.begin(); 

  //Setup BNO080 to use SPI1 interface with max BNO080 clk speed of 3MHz
  imu_initialized = myIMU.beginSPI(imuCSPin, imuWAKPin, imuINTPin, imuRSTPin, 3000000, SPI1); //changed from slaveCPin

  // Default periodicity (IMU_REFRESH_PERIOD ms)
  if (imu_initialized)
  {
    myIMU.enableLinearAccelerometer(50); // m/s^2 no gravity
    myIMU.enableRotationVector(50);  // quat
    myIMU.enableGyro(50); // rad/s
    //myIMU.enableMagnetometer(50); //  cannot be enabled at the same time as RotationVector (will not produce data)

    Serial.println(F("LinearAccelerometer enabled, Output in form x, y, z, in m/s^2"));
    Serial.println(F("Gyro enabled, Output in form x, y, z, in radians per second"));
    Serial.println(F("Rotation vector, Output in form i, j, k, real, accuracy"));
    //Serial.println(F("Magnetometer enabled, Output in form x, y, z, in uTesla"));
  }
}


void loop() {

  //Look for data from the IMU
  if (imu_initialized)
  {
    if(!imu_calibrated)
    {
      myIMU.requestCalibrationStatus();
      if(myIMU.calibrationComplete() == true)
      {
        Serial.println("Device is calibrated");
        imu_calibrated = true;
      }
    }
  
    if (myIMU.dataAvailable() == true)
    {
      byte linAccuracy = 0;
      byte gyroAccuracy = 0;
      byte magAccuracy = 0;
      float quatRadianAccuracy = 0;
      byte quatAccuracy = 0;
    
      myIMU.getLinAccel(ax, ay, az, linAccuracy);
      myIMU.getGyro(gx, gy, gz, gyroAccuracy);
      myIMU.getQuat(qx, qy, qz, qw, quatRadianAccuracy, quatAccuracy);
      //myIMU.getMag(mx, my, mz, magAccuracy);
      
      
      if (imu_calibrated)
      {
        Serial.print(F("acc :"));
        Serial.print(ax, 2);
        Serial.print(F(","));
        Serial.print(ay, 2);
        Serial.print(F(","));
        Serial.print(az, 2);
        Serial.print(F(","));
        Serial.print(az, 2);
        Serial.print(F(","));
        printAccuracyLevel(linAccuracy);
         
        Serial.print(F("gyro:"));
        Serial.print(gx, 2);
        Serial.print(F(","));
        Serial.print(gy, 2);
        Serial.print(F(","));
        Serial.print(gz, 2);
        Serial.print(F(","));
        printAccuracyLevel(gyroAccuracy);
 /*        
        Serial.print(F("mag :"));
        Serial.print(mx, 2);
        Serial.print(F(","));
        Serial.print(my, 2);
        Serial.print(F(","));
        Serial.print(mz, 2);
        Serial.print(F(","));
        printAccuracyLevel(magAccuracy);
*/
       
        Serial.print(F("quat:"));
        Serial.print(qx, 2);
        Serial.print(F(","));
        Serial.print(qy, 2);
        Serial.print(F(","));
        Serial.print(qz, 2);
        Serial.print(F(","));
        Serial.print(qw, 2);
        Serial.print(F(","));
        printAccuracyLevel(quatAccuracy);
        
      }
    }
  }
  else
  {
    Serial.println("BNO080 not detected. Check your jumpers and the hookup guide. Freezing...");
    while (1);
  }
}

//Given a accuracy number, print what it means
void printAccuracyLevel(byte accuracyNumber)
{
  if (accuracyNumber == 0) Serial.println(F("Unreliable"));
  else if (accuracyNumber == 1) Serial.println(F("Low"));
  else if (accuracyNumber == 2) Serial.println(F("Medium"));
  else if (accuracyNumber == 3) Serial.println(F("High"));
}

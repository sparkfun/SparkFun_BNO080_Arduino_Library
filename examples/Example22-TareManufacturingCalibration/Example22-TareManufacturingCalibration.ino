/*
  Using the BNO080 IMU
  By: Nathan Seidle
  SparkFun Electronics
  Date: December 21st, 2017
  SparkFun code, firmware, and software is released under the MIT License.
	Please see LICENSE.md for further details.

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14586

  This example shows how to output the i/j/k/real parts of the rotation vector.
  https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation

  It takes about 1ms at 400kHz I2C to read a record from the sensor, but we are polling the sensor continually
  between updates from the sensor. Use the interrupt pin on the BNO080 breakout to avoid polling.

  Hardware Connections:
  Attach the Qwiic Shield to your Arduino/Photon/ESP32 or other
  Plug the sensor onto the shield
  Serial.print it out at 9600 baud to serial monitor.
*/

#include <Wire.h>

#include "SparkFun_BNO080_Arduino_Library.h" // Click here to get the library: http://librarymanager/All#SparkFun_BNO080
BNO080 myIMU;

void setup()
{
  Serial.begin(9600);
  Serial.println();
  Serial.println("BNO080 Read Example");

  Wire.begin();

  //Are you using a ESP? Check this issue for more information: https://github.com/sparkfun/SparkFun_BNO080_Arduino_Library/issues/16
//  //=================================
//  delay(100); //  Wait for BNO to boot
//  // Start i2c and BNO080
//  Wire.flush();   // Reset I2C
//  IMU.begin(BNO080_DEFAULT_ADDRESS, Wire);
//  Wire.begin(4, 5);
//  Wire.setClockStretchLimit(4000);
//  //=================================

  if (myIMU.begin() == false)
  {
    Serial.println("BNO080 not detected at default I2C address. Check your jumpers and the hookup guide. Freezing...");
    while (1);
  }

  Wire.setClock(400000); //Increase I2C data rate to 400kHz

  myIMU.enableRotationVector(50); //Send data update every 50ms

  Serial.println(F("Rotation vector enabled"));
  Serial.println(F("Output in form i, j, k, real, accuracy"));
}

void loop()
{
  if(Serial.available())
  {
  byte incoming = Serial.read();

  if(incoming == 't')
  {
    myIMU.tareAllAxes(); //tare x,y,z axes to calibrate device with default manufacturing
    myIMU.saveTare(); //save tare
    delay(1);
    }
  }
  
  //Look for reports from the IMU
  if (myIMU.dataAvailable() == true)
  {
    float quatI = myIMU.getQuatI();
    float quatJ = myIMU.getQuatJ();
    float quatK = myIMU.getQuatK();
    float quatReal = myIMU.getQuatReal();
    float quatRadianAccuracy = myIMU.getQuatRadianAccuracy();

   /* See page 5 of Tare 
    * Determine North in your current environment
    * a. You can either look at a physical compass, or look at the Rotation Vector to
    * determine where North is. 
    * When the Rotation Vector reads W=1, X=0, Y=0, Z=0 the device is pointed North

    *    w = real
    *     x = i
    *     y = j
    *     z = k
    */

    Serial.print(F(", W : "));
    Serial.print(quatReal, 2);
    Serial.print(F(", x : "));
    Serial.print(quatI, 2);
    Serial.print(F(", y: "));
    Serial.print(quatJ, 2);
    Serial.print(F(", z: "));
    Serial.print(quatK, 2);
    Serial.print(F(","));
    Serial.print(quatRadianAccuracy, 2);
    Serial.print(F(","));

    Serial.println();
  }
}

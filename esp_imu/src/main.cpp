#include <Arduino.h>
#include<Wire.h>
#define MPUaddr 0x68

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(MPUaddr);
  byte busStatus = Wire.endTransmission();
  if (busStatus != 0)
  {
    Serial.print("MPU is not found!");
    while (1); //wait for ever
  }
  Serial.println("MPU is found");
}

void loop()
{
  Wire.beginTransmission(MPUaddr);                        //Start communication with the MPU-6050.
  Wire.write(0x6B);                                            //We want to write to the PWR_MGMT_1 register (6B hex).
  Wire.write(0x00);   //enable temp sensor                                           //Set the register bits as 00000000 to activate the gyro.
  Wire.endTransmission();

  //----pointing temp sensor-----------------
  Wire.beginTransmission(MPUaddr);                        //Start communication with the MPU-6050.
  Wire.write(0x41);      //pointing Temp_Out_High Reg                                           //Set the register bits as 00000000 to activate the gyro.
  Wire.endTransmission();

  Wire.requestFrom(MPUaddr, 2); //two-byte temp data                                  //End the transmission with the gyro.
  byte x1 = Wire.read();
  byte x2 = Wire.read();
  int x = (int)(x1 << 8) | (int)x2;

  //------compute temp from x-----
  float mpuTemp = (float)(x / 340.0 + 36.53); //formula from data sheets
  Serial.print("Temp = ");
  Serial.print(mpuTemp, 2);  //2-digit after decimal point
  Serial.println(" degC");
  delay(1000);
}
#define SDA_PORT PORTC
#define SOFT_SDA_PIN 4 // = A4
#define SCL_PORT PORTC
#define SOFT_SCL_PIN 5 // = A5

#include <SoftWire.h>
#define MPU_addr 0x68
const int MIC_SENSOR_PINT = A2;

SoftWire Wire = SoftWire();


void initCowSensors()
{
    Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);


}


int readMic()
{
   int mic = analogRead(MIC_SENSOR_PINT);
   return mic;
}


bool  readSensors()
{
  static int  count =0;
  bool ret = false;

    if(count ==0 )
  {
    sensorsArray[sensorArrayCounter].aX = 0;
    sensorsArray[sensorArrayCounter].aY = 0;
    sensorsArray[sensorArrayCounter].aZ = 0;
    sensorsArray[sensorArrayCounter].temp = 0;
    sensorsArray[sensorArrayCounter].gX = 0;
    sensorsArray[sensorArrayCounter].gY = 0;
    sensorsArray[sensorArrayCounter].gZ = 0;
    sensorsArray[sensorArrayCounter].mic = 0;
  }

  
  if(count < AVERAGE_SIZE_SEC)
  {
   int mic= readMic();
      Wire.beginTransmission(MPU_addr);
     // Serial.println(mic);
       //  Serial.flush();
    Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
    sensorsArray[sensorArrayCounter].aX += (Wire.read()<<8|Wire.read())/AVERAGE_SIZE_SEC;  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
    sensorsArray[sensorArrayCounter].aY += (Wire.read()<<8|Wire.read())/AVERAGE_SIZE_SEC;  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    sensorsArray[sensorArrayCounter].aZ += (Wire.read()<<8|Wire.read())/AVERAGE_SIZE_SEC;  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    sensorsArray[sensorArrayCounter].temp += (Wire.read()<<8|Wire.read())/AVERAGE_SIZE_SEC;  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    sensorsArray[sensorArrayCounter].gX += (Wire.read()<<8|Wire.read())/AVERAGE_SIZE_SEC;  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    sensorsArray[sensorArrayCounter].gY += (Wire.read()<<8|Wire.read())/AVERAGE_SIZE_SEC;  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    sensorsArray[sensorArrayCounter].gZ += (Wire.read()<<8|Wire.read())/AVERAGE_SIZE_SEC;  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
    sensorsArray[sensorArrayCounter].mic += mic/AVERAGE_SIZE_SEC;
    count++;
  }
  else
  {
    sensorsArray[sensorArrayCounter].temp = (sensorsArray[sensorArrayCounter].temp)/340.00+36.53;
    count =0;
    ret = true;
  }

return ret;
}
String getSensorString(int index)
{
  String sensorString = "";
  
  sensorString += String(sensorsArray[index].mic);
  sensorString += "\t";
  sensorString += String(sensorsArray[index].temp,2);
  sensorString += "\t";
  sensorString += String(sensorsArray[index].aX,0);
  sensorString += "\t";
  sensorString += String(sensorsArray[index].aY,0);
  sensorString += "\t";
  sensorString += String(sensorsArray[index].aZ,0);
  sensorString += "\t";
  sensorString += String(sensorsArray[index].gX);
  sensorString += "\t";
  sensorString += String(sensorsArray[index].gY,0);
  sensorString += "\t";
  sensorString += String(sensorsArray[index].gZ,0);
  sensorString += "\r\n";
  return sensorString;
 }

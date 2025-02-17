#include <Wire.h>
#include "Accelerometer.h"
#include "Gyroscope.h"
#include "Magnetometer.h"

struct Sensor {
  // define a single structure named 'Sensor' that contain accelerometer, gyroscope and magnetometer.

  Accelerometer accelerometer;
  Gyroscope gyroscope;
  Magnetometer magnetometer;
  uint8_t MPU6050_ADDRESS;

  Sensor(uint8_t MPU6050_ADDRESS_, uint8_t ACC_GYRO_DLPF_FRQ_, uint8_t FSR_ACC_, uint8_t FSR_GYRO_,
         uint8_t HMC5883_ADDRESS_, uint8_t MAG_DOUTPUT_RATE_, uint8_t MAG_GAIN_, uint8_t MAG_MEASUREMENT_MODE_)
    : accelerometer(MPU6050_ADDRESS_, ACC_GYRO_DLPF_FRQ_, FSR_ACC_), gyroscope(MPU6050_ADDRESS_, ACC_GYRO_DLPF_FRQ_, FSR_GYRO_),
      magnetometer(HMC5883_ADDRESS_, MAG_DOUTPUT_RATE_, MAG_GAIN_, MAG_MEASUREMENT_MODE_) {

    MPU6050_ADDRESS = MPU6050_ADDRESS_;
  };

  void start(int MPU_6050_POWER_MODE_) {

    Wire.beginTransmission(MPU6050_ADDRESS);  // start the I2C communication (page 45)
    Wire.write(0x6B);                         // start the gyro power register in power mode (page 40)
    Wire.write(MPU_6050_POWER_MODE_);         // command to maintain the power mode (page 40)
    Wire.endTransmission();                   // terminate I2C communication
  }
};

void TCA9548A_set_path(uint8_t bus)
{
  Wire.beginTransmission(0x70);  // TCA9548A address is 0x70
  Wire.write(1 << bus);          // send byte to select bus
  Wire.endTransmission();
}


Sensor SENSOR1(0x68, 0x05, 0x10, 0x10, 0x1E, 0x14, 0x20, 0x00);
Sensor SENSOR2(0x69, 0x05, 0x10, 0x10, 0x1E, 0x14, 0x20, 0x00);

char recievedChar;
String output = "";

void setup() {
  Serial.begin(115200);
  Wire.setClock(400000);  // set I2C clock speed (page 5)
  Wire.begin();
  
  TCA9548A_set_path(1);
  SENSOR1.start(0x00);
  TCA9548A_set_path(2);
  SENSOR2.start(0x00);
}

void loop() {
  if (Serial.available()) {
    recievedChar = Serial.read();
    if (recievedChar == '1') {
      TCA9548A_set_path(1)
      output = getFormatedOutput(output, SENSOR1.accelerometer.getReadings(), false);
      output = getFormatedOutput(output, SENSOR1.gyroscope.getReadings(), false);
      output = getFormatedOutput(output, SENSOR1.magnetometer.getReadings(), true);
      Serial.println(output);

    } else if (recievedChar == '2') {
      TCA9548A_set_path)(2)
      output = getFormatedOutput(output, SENSOR2.accelerometer.getReadings(), false);
      output = getFormatedOutput(output, SENSOR2.gyroscope.getReadings(), false);
      output = getFormatedOutput(output, SENSOR2.magnetometer.getReadings(), true);
      Serial.println(output);
    }
    output = "";
  }
}

String getFormatedOutput(String output_, float arr_[3], bool isEnd) {
  for (int i = 0; i < 3; i++) {
    output_ += String(arr_[i]) + ",";
  }
  if (isEnd) {
    return output_.substring(0, output_.length() - 1);
  }
  return output_;
}
#include "mpu6050.h"
#include <Wire.h>
#include <math.h>

const int MPU_ADDR = 0x68;
unsigned long lastMs = 0;
float currentYaw = 0.0;

void initImu(int sda, int scl) {
  Wire.begin(sda, scl);
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission();
  lastMs = millis();
}

ImuReading readImu() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 14, true);

  int16_t ax = Wire.read() << 8 | Wire.read();
  int16_t ay = Wire.read() << 8 | Wire.read();
  int16_t az = Wire.read() << 8 | Wire.read();
  Wire.read(); Wire.read();
  Wire.read(); Wire.read();
  Wire.read(); Wire.read();
  int16_t gz = Wire.read() << 8 | Wire.read();

  unsigned long now = millis();
  float dt = (now - lastMs) / 1000.0;
  if (dt <= 0.0 || dt > 0.5) dt = 0.02;
  lastMs = now;

  float pitch = atan2(-(float)ax, sqrt((float)ay * ay + (float)az * az)) * 180.0 / 3.14159;
  float gzRate = (float)gz / 131.0;
  if (fabs(gzRate) > 1.2) currentYaw += gzRate * dt;

  ImuReading data;
  data.pitch = pitch;
  data.yaw = currentYaw;
  return data;
}

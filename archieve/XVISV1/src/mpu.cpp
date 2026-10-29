#include "mpu.h"
#include <Wire.h>
#include <math.h>

static const uint8_t MPU_ADDR = 0x68;
static unsigned long lastMs = 0;
static float currYaw = 0.0f;

void mpuInit(int sda, int scl) {
  Wire.begin(sda, scl);
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();
  lastMs = millis();
}

void mpuRead(float& pitch, float& yaw) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  if (Wire.endTransmission(false) != 0) {
    pitch = 0.0f;
    yaw = currYaw;
    return;
  }

  if (Wire.requestFrom((uint16_t)MPU_ADDR, (uint8_t)14, true) != 14) {
    pitch = 0.0f;
    yaw = currYaw;
    return;
  }

  int16_t ax = (Wire.read() << 8) | Wire.read();
  int16_t ay = (Wire.read() << 8) | Wire.read();
  int16_t az = (Wire.read() << 8) | Wire.read();
  Wire.read(); Wire.read();
  Wire.read(); Wire.read();
  Wire.read(); Wire.read();
  int16_t gz = (Wire.read() << 8) | Wire.read();

  unsigned long now = millis();
  float dt = (now - lastMs) / 1000.0f;
  if (dt <= 0.0f || dt > 0.5f) dt = 0.02f;
  lastMs = now;

  float ax_g = (float)ax / 16384.0f;
  float ay_g = (float)ay / 16384.0f;
  float az_g = (float)az / 16384.0f;
  float gz_d = (float)gz / 131.0f;

  pitch = atan2(-ax_g, sqrt(ay_g * ay_g + az_g * az_g)) * 180.0f / 3.14159f;

  if (fabs(gz_d) > 1.2f) {
    currYaw += gz_d * dt;
  }
  if (currYaw >= 360.0f) currYaw -= 360.0f;
  if (currYaw < 0.0f) currYaw += 360.0f;
  yaw = currYaw;
}


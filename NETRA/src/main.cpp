#include <Arduino.h>
#include "sonar.h"
#include "mpu.h"
#include "uart.h"

float dists[SENSOR_COUNT];
int servos[SENSOR_COUNT];

void setup() {
  Serial.begin(115200);

  sonarInit();
  mpuInit(21, 22);
  uartInit(17, 115200);
}

void loop() {
  sonarReadAll(dists);

  for (int i = 0; i < SENSOR_COUNT; i++) {
    servos[i] = calcHapticAngle(dists[i]);
  }

  uartSendHaptics(servos, SENSOR_COUNT);

  float pitch = 0.0f;
  float yaw = 0.0f;
  mpuRead(pitch, yaw);

  Serial.printf("%.1f,%.1f,%.1f,%.1f,%.1f,%d,%d,%d,%d,%d,%.1f,%.1f\n",
    dists[0], dists[1], dists[2], dists[3], dists[4],
    servos[0], servos[1], servos[2], servos[3], servos[4],
    pitch, yaw);

  delay(15);
}


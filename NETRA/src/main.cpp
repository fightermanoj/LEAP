#include <Arduino.h>
#include "pins.h"
#include "mpu6050.h"

float dists[5] = {60, 60, 60, 60, 60};
int servos[5] = {180, 180, 180, 180, 180};

float getDistance(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  unsigned long dur = pulseIn(echo, HIGH, 25000);
  if (dur == 0) return 60.0;
  float d = dur * 0.0343 / 2.0;
  return constrain(d, 2.0, 60.0);
}

int getAngle(float d) {
  if (d >= 60.0) return 180;
  if (d <= 5.0) return 140;
  return map((long)(d * 10), 50, 600, 140, 180);
}

void syncServos() {
  Serial2.printf("$%d,%d,%d,%d,%d*\n", servos[0], servos[1], servos[2], servos[3], servos[4]);
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, -1, uartTxPin);

  for (int i = 0; i < 5; i++) {
    pinMode(trigPins[i], OUTPUT);
    digitalWrite(trigPins[i], LOW);
    pinMode(echoPins[i], INPUT);
  }

  initImu(sdaPin, sclPin);
}

void loop() {
  for (int i = 0; i < 5; i++) {
    dists[i] = getDistance(trigPins[i], echoPins[i]);
    servos[i] = getAngle(dists[i]);
    delay(5);
  }

  syncServos();
  ImuReading imu = readImu();

  Serial.printf("%.1f,%.1f,%.1f,%.1f,%.1f,%d,%d,%d,%d,%d,%.1f,%.1f\n",
    dists[0], dists[1], dists[2], dists[3], dists[4],
    servos[0], servos[1], servos[2], servos[3], servos[4],
    imu.pitch, imu.yaw);

  delay(10);
}

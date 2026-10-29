#include "sonar.h"

void sonarInit() {
  for (int i = 0; i < SENSOR_COUNT; i++) {
    pinMode(trigPins[i], OUTPUT);
    digitalWrite(trigPins[i], LOW);
    pinMode(echoPins[i], INPUT);
  }
}

float sonarRead(int index) {
  if (index < 0 || index >= SENSOR_COUNT) return 400.0f;

  int trig = trigPins[index];
  int echo = echoPins[index];

  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  unsigned long dur = pulseIn(echo, HIGH, 30000);
  if (dur == 0) return 400.0f;

  float d = (dur * 0.0343f) / 2.0f;
  if (d < 2.0f || d > 400.0f) return 400.0f;
  return d;
}

void sonarReadAll(float* dists) {
  for (int i = 0; i < SENSOR_COUNT; i++) {
    dists[i] = sonarRead(i);
    delay(5);
  }
}

int calcHapticAngle(float dist) {
  if (dist >= 60.0f) return 180;
  if (dist <= 5.0f) return 140;
  return map((long)(dist * 10), 50, 600, 140, 180);
}


#pragma once
#include <Arduino.h>

const int SENSOR_COUNT = 5;

const int trigPins[SENSOR_COUNT] = {33, 25, 26, 27, 14};
const int echoPins[SENSOR_COUNT] = {36, 39, 34, 35, 32};

void sonarInit();
float sonarRead(int index);
void sonarReadAll(float* dists);
int calcHapticAngle(float dist);


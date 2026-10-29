#pragma once
#include <Arduino.h>

const int SENSOR_COUNT = 5;

const int trigPins[SENSOR_COUNT] = {12, 14, 25, 27, 26};
const int echoPins[SENSOR_COUNT] = {36, 39, 32, 34, 35};

void sonarInit();
float sonarRead(int index);
void sonarReadAll(float* dists);
int calcHapticAngle(float dist);

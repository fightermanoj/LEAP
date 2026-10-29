#pragma once
#include <Arduino.h>

void mpuInit(int sda = 21, int scl = 22);
void mpuRead(float& pitch, float& yaw);


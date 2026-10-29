#pragma once
#include <Arduino.h>

void uartInit(int txPin = 17, long baud = 115200);
void uartSendHaptics(const int* angles, int count = 5);


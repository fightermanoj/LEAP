#include "uart.h"

void uartInit(int txPin, long baud) {
  Serial2.begin(baud, SERIAL_8N1, -1, txPin);
}

void uartSendHaptics(const int* angles, int count) {
  Serial2.print("$");
  for (int i = 0; i < count; i++) {
    Serial2.print(angles[i]);
    if (i < count - 1) Serial2.print(",");
  }
  Serial2.println("*");
}


#include <Arduino.h>

const int trig = 18;
const int echoPin = 19;
const int sPin = 13;
const int sChan = 0;

int toDuty(int ang) {
  long us = map(ang, 0, 180, 500, 2400);
  return (us * 65535) / 20000;
}

long getDist() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long dur = pulseIn(echoPin, HIGH, 25000);
  if (dur == 0) return 400;
  return (dur * 0.0343) / 2;
}

void setup() {
  Serial.begin(115200);
  pinMode(trig, OUTPUT);
  pinMode(echoPin, INPUT);
  ledcSetup(sChan, 50, 16);
  ledcAttachPin(sPin, sChan);
}

void loop() {
  for (int a = 0; a <= 180; a += 3) {
    ledcWrite(sChan, toDuty(a));
    delay(20);
    long d = getDist();
    Serial.printf("%d.%ld\n", a, d);
  }
  for (int a = 180; a >= 0; a -= 3) {
    ledcWrite(sChan, toDuty(a));
    delay(20);
    long d = getDist();
    Serial.printf("%d.%ld\n", a, d);
  }
}



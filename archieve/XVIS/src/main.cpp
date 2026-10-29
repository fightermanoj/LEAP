#include <Arduino.h>

const int trigPin = 18;
const int echoPin = 19;
const int servoPin = 13;
const int pwmChan = 0;
bool running = true;

int toDuty(int ang) {
  long pulse = map(ang, 0, 180, 500, 2400);
  return (pulse * 65535) / 20000;
}

void triggerPulse() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
}

long getDist() {
  triggerPulse();
  long dur = pulseIn(echoPin, HIGH, 25000);
  if (dur == 0) return 400;
  return (dur * 0.0343) / 2;
}

void checkSerial() {
  if (Serial.available()) {
    char c = Serial.read();
    if (c == '0') running = false;
    if (c == '1') running = true;
  }
}

void sweepStep(int ang) {
  checkSerial();
  if (!running) return;
  ledcWrite(pwmChan, toDuty(ang));
  delay(25);
  long dist = getDist();
  Serial.printf("%d.%ld\n", ang, dist);
}

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  ledcSetup(pwmChan, 50, 16);
  ledcAttachPin(servoPin, pwmChan);
}

void loop() {
  checkSerial();
  if (!running) {
    delay(50);
    return;
  }
  for (int a = 0; a <= 180; a += 1) {
    if (!running) break;
    sweepStep(a);
  }
  for (int a = 180; a >= 0; a -= 1) {
    if (!running) break;
    sweepStep(a);
  }
}

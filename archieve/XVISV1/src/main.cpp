#include <Arduino.h>

const int trigPins[5] = {33, 25, 26, 27, 14};
const int echoPins[5] = {36, 39, 34, 35, 32};
const int servoPin = 23;
const int pwmChan = 0;

const int pwmFreq = 50;
const int pwmResolution = 16;
const unsigned long timeoutUs = 25000;

int currentAngle = 180;
float distances[5] = {60, 60, 60, 60, 60};

uint32_t degreesToDuty(int deg) {
  deg = constrain(deg, 0, 180);
  long pw = map(deg, 0, 180, 500, 2400);
  return (uint32_t)((pw * 65535LL) / 20000);
}

float readSensor(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  unsigned long dur = pulseIn(echo, HIGH, timeoutUs);
  if (dur == 0) return 60.0f;
  float d = dur * 0.0343f / 2.0f;
  return constrain(d, 2.0f, 60.0f);
}

int calcTargetAngle(float closest) {
  if (closest >= 60.0f) return 180;
  if (closest <= 5.0f) return 140;
  return map((long)(closest * 10), 50, 600, 140, 180);
}

void stepServo(int target) {
  if (currentAngle < target) currentAngle++;
  else if (currentAngle > target) currentAngle--;
  ledcWrite(pwmChan, degreesToDuty(currentAngle));
}

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < 5; i++) {
    pinMode(trigPins[i], OUTPUT);
    digitalWrite(trigPins[i], LOW);
    pinMode(echoPins[i], INPUT);
  }

  ledcSetup(pwmChan, pwmFreq, pwmResolution);
  ledcAttachPin(servoPin, pwmChan);
  ledcWrite(pwmChan, degreesToDuty(180));
}

void loop() {
  float closest = 60.0f;

  for (int i = 0; i < 5; i++) {
    distances[i] = readSensor(trigPins[i], echoPins[i]);
    if (distances[i] < closest) closest = distances[i];
    delay(10);
  }

  int targetAngle = calcTargetAngle(closest);
  stepServo(targetAngle);

  Serial.printf("%.1f,%.1f,%.1f,%.1f,%.1f,%d\n",
    distances[0], distances[1], distances[2], distances[3], distances[4], currentAngle);

  delay(15);
}

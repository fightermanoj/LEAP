#include <Arduino.h>

const int trigPin = 18;
const int echoPin = 19;
const int servoPin = 26;
const int pwmChan = 0;

const int pwmFreq = 50;
const int pwmResolution = 16;

const int minDistance = 3;
const int maxDistance = 60;

const int minAngle = 140;
const int maxAngle = 180;

unsigned long timeoutUs = 30000;
int currentAngle = 180;

uint32_t degreesToDutyCycle(int degrees) {
  degrees = constrain(degrees, 0, 180);
  long pulseWidth = map(degrees, 0, 180, 500, 2400);
  return (uint32_t)((pulseWidth * 65535LL) / 20000);
}

float readDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long duration = pulseIn(echoPin, HIGH, timeoutUs);
  if (duration == 0) return -1;
  return duration * 0.0343f / 2.0f;
}

void setup() {
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  ledcSetup(pwmChan, pwmFreq, pwmResolution);
  ledcAttachPin(servoPin, pwmChan);
  ledcWrite(pwmChan, degreesToDutyCycle(180));

  Serial.println("Ultrasonic-to-servo mapping ready");
}

void loop() {
  float distance = readDistance();

  if (distance < 0) {
    delay(20);
    return;
  }

  int targetAngle;
  if (distance >= 60) {
    targetAngle = 180;
  } else if (distance <= 5) {
    targetAngle = 140;
  } else {
    targetAngle = map((long)(distance * 10), 50, 600, 140, 180);
  }

  if (currentAngle < targetAngle) currentAngle++;
  else if (currentAngle > targetAngle) currentAngle--;

  ledcWrite(pwmChan, degreesToDutyCycle(currentAngle));

  Serial.print("Distance: ");
  Serial.print(distance, 1);
  Serial.print(" cm | Target: ");
  Serial.print(targetAngle);
  Serial.print(" deg | Servo: ");
  Serial.println(currentAngle);

  delay(20);
}

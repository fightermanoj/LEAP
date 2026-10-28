#include <ESP32Servo.h>

const int trigPin = 5;
const int echoPin = 18;
const int servoPin = 13;

Servo servo;

void setup(){
    Serial.begin(115200);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    ESP32PWM::allocateTimer(0);
    scanServo.setPreiodHertz(50);
    scanServo.attach(servoPin, 500, 2400);
}

float getDistance(){
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH, 30000);
    if (duration == 0){
        return -1;
    }

    float distance = duration * 0.034/2;
    return distance;
}

void loop(){
    for (int angle = 0; angle <= 180; angle += 5){
        scanServo.write(angle);
        delay(50);

        float distance = getDistance();

        if (distance > 0){

        }
    }
}

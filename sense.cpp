#include <ESP32Servo.h>

#define TRIG_PIN  5
#define ECHO_PIN  18
#define SERVO_PIN  13
#define ALLOWED_DIST  100 /*in cm*/
#define ALERT_DIST  30

Servo s;

void setup(){
    Serial.begin(115200);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    s.setPeriodHertz(50);
    s.attach(SERVO_PIN);
    s.write(0);
}

long get_distance(){
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH, 30000);
    long distance = (duration * 0.0343)/2;

    if (distance == 0 || distance > 400)
        distance = 400;

    return distance;
}

void alert_servo(long dist){
    if distance >= ALLOWED_DIST
        servo.write(0);
    
    int tap_delay = map(dist, ALLOWED_DIST, ALERT_DIST, 50, 400);
    tap_delay = constrain(tap_delay, 50, 400);

    
}
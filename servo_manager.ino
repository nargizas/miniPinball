#include <Servo.h> 

Servo leftServo;
Servo rightServo;
#define LEFT_SERVO 10
#define RIGHT_SERVO 11
#define RIGHT_JOY A0

void setup() {
  leftServo.attach(LEFT_SERVO);
  rightServo.attach(RIGHT_SERVO);
}

void loop() {

    //move flippers according to reading from the joystick
    int inputValue = analogRead(RIGHT_JOY);
    int angle = map(inputValue, 0, 1023, -30, 30);
    leftServo.write(angle*3+90);
    rightServo.write(90-angle*3);
}

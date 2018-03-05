#include <Servo.h>
#include <SoftwareSerial.h>
#include <ArduinoBlue.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"

/**
 * Pins
 */
// continuous microservos
const int leftServoPin = 11;
const int rightServoPin = 10;

// Bluetooth module HM 10 pins
const int BLUETOOTH_TX = 8;
const int BLUETOOTH_RX = 7;


/*
 * Continuous rotation micro servo parameters.
 */
// If the throttle value is within 5 from zero, then it will be considered 0.
const int THROTTLE_ZERO_THRESHOLD = 5;
const int MIN_THROTTLE = 10;
const int STILL = 90;
const int LEFT_FORWARD_MAX = 130;
const int LEFT_BACKWARD_MAX = 50;
const int RIGHT_FORWARD_MAX = LEFT_BACKWARD_MAX;
const int RIGHT_BACKWARD_MAX = LEFT_FORWARD_MAX;
const int LEFT_FORWARD_MIN = STILL + MIN_THROTTLE;
const int LEFT_BACKWARD_MIN = STILL - MIN_THROTTLE;
const int RIGHT_FORWARD_MIN = LEFT_BACKWARD_MIN;
const int RIGHT_BACKWARD_MIN = LEFT_FORWARD_MIN;

/**
 * Variables
 */
Servo leftServo;
Servo rightServo;

SoftwareSerial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX);
ArduinoBlue phone(bluetooth);


/**
 * Functions
 */
// Re-maps a float number from one range to another.
// That is, a value of fromLow would get mapped to toLow, a value of fromHigh to toHigh, values in-between to values in-between, etc.
float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// This function handles the driving.
// Input is throttle and steering values from the phone.
void handleDriving(int throttle, int steering) {
  // Refer to comment in setup() function.
  // Take away 49 from both so that
  // throttle 0 -> no throttle
  // steering 0 -> no turn (straight)
  throttle -= 49;
  steering -= 49;

  int leftVal, rightVal;
  double throttleReduction;

  // If the throttle is close enough to zero, the robot stays still.
  if (abs(throttle) < THROTTLE_ZERO_THRESHOLD) {
    leftVal = STILL;
    rightVal = STILL;
  }
  // If the throttle is greater than zero, then the robot goes forward
  else if (throttle > 0) {
    // turn left forward
    if (steering <= 0) {
      // reduce throttle in the left servo to go left forwards.
      throttleReduction = mapFloat(steering, 0, -49, 1, 0); 
      leftVal = map(throttle*throttleReduction, 0, 50, LEFT_FORWARD_MIN, LEFT_FORWARD_MAX);
      rightVal = map(throttle, 0, 50, RIGHT_FORWARD_MIN, RIGHT_FORWARD_MAX);
    }
    // turn right forward
    else {
      // reduce throttle in the right servo to go right forward.
      throttleReduction = mapFloat(steering, 1, 50, 1, 0); 
      leftVal = map(throttle, 0, 50, LEFT_FORWARD_MIN, LEFT_FORWARD_MAX);
      rightVal = map(throttle*throttleReduction, 0, 50, RIGHT_FORWARD_MIN, RIGHT_FORWARD_MAX);
    }
  }
  // backward
  else {
    // TODO: implement this
    // turn left backward
    if (steering <= 0) {
      // reduce throttle in the left servo to go left backwards.
      throttleReduction = mapFloat(steering, 0, -49, 1, 0); 
      leftVal = map(throttle*throttleReduction, 0, -49, LEFT_BACKWARD_MIN, LEFT_BACKWARD_MAX);
      rightVal = map(throttle, 0, -49, RIGHT_BACKWARD_MIN, RIGHT_BACKWARD_MAX);
    }
    // turn right backward
    else {
      // reduce throttle in the right servo to go right backwards.
      throttleReduction = mapFloat(steering, 1, 50, 1, 0); 
      leftVal = map(throttle, 0, -49, LEFT_BACKWARD_MIN, LEFT_BACKWARD_MAX);
      rightVal = map(throttle*throttleReduction, 0, -49, RIGHT_BACKWARD_MIN, RIGHT_BACKWARD_MAX);
    }
  }

  // write the servo values to the servos.
  leftServo.write(leftVal);
  rightServo.write(rightVal);

  /*Serial.print("left = "); Serial.print(leftVal);
  Serial.print(" | right = "); Serial.print(rightVal);
  Serial.print(" | r = "); Serial.print(throttleReduction);
  Serial.print(" | steering = "); Serial.print(steering);
  Serial.print(" | throttle = "); Serial.println(throttle); */
}

void setup() {
  // Begin serial communication at 9600 bps
  Serial.begin(9600);

  // Begin serial communication with HM 10 at 9600 bps 
  bluetooth.begin(9600);

  // delay in case the HM 10 needs time to respond
  delay(100);

  // Attach the servos
  leftServo.attach(leftServoPin);
  rightServo.attach(rightServoPin);
  
  Serial.println("Setup Complete");
}


void loop() {
  // throttle and steering values go from 0 to 99
  // throttle 0 -> maximum backwards throttle
  // throttle 49 -> no throttle (stay still)
  // throttle 99 -> maximum forwards throttle
  // steering 0 -> maximum left turn
  // steering 49 -> no turn (straight)
  // steering 99 -> maximum right turn
  int throttle = phone.getThrottle();
  int steering = phone.getSteering();
  handleDriving(throttle, steering);
}

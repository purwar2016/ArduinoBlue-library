#include <Servo.h>
#include <SoftwareSerial.h>
#include <ArduinoBlue.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"

// Pick analog outputs, for the UNO these three work well
// use ~560  ohm resistor between Red & Blue, ~1K for green (its brighter)
#define redpin 13
#define greenpin 12
#define bluepin 4
// for a common anode LED, connect the common pin to +5V
// for common cathode, connect the common to ground

// continuous microservos
#define leftServoPin 11
#define rightServoPin 10

// Bluetooth module HM 10 pins
#define BLUETOOTH_TX 8
#define BLUETOOTH_RX 7

// set to false if using a common cathode LED
#define commonAnode true

// RGB 0 - 255 if this threshold is reached then it is considered equal
#define COLOR_THRESHOLD 15

// Continuous rotation micro servo parameters.
#define THROTTLE_ZERO_THRESHOLD 5
#define MIN_THROTTLE 10
#define STILL 90
#define LEFT_FORWARD_MAX 130
#define LEFT_BACKWARD_MAX 50
#define RIGHT_FORWARD_MAX LEFT_BACKWARD_MAX
#define RIGHT_BACKWARD_MAX LEFT_FORWARD_MAX
#define LEFT_FORWARD_MIN STILL + MIN_THROTTLE
#define LEFT_BACKWARD_MIN STILL - MIN_THROTTLE
#define RIGHT_FORWARD_MIN LEFT_BACKWARD_MIN
#define RIGHT_BACKWARD_MIN LEFT_FORWARD_MIN

// RGB values for various plastic surface colors
const uint8_t RED_RGB[] = {168, 50, 54}; // blue
const uint8_t GREEN_RGB[] = {70, 125, 62};  //red
const uint8_t BLUE_RGB[] = {53, 96, 111}; // green

enum Color { RED, GREEN, BLUE, OTHER };

String getColorString(Color color) {
  if (color == RED) {
        return "red";
  }
  else if (color == GREEN) {
    return "green"; 
  }
  else if (color == BLUE) {
    return "blue";
  }
  else if (color == OTHER) {
    return "other"; 
  }
}

// Controls the color sensor LED
bool ledOff = false;

Servo leftServo;
Servo rightServo;

SoftwareSerial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX);
ArduinoBlue phone(bluetooth);

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

Color currentColor, previousColor;

float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

bool colorCompare(int r, int g, int b, const uint8_t ary[]) {
  if (abs(ary[0] - r) > COLOR_THRESHOLD
      || abs(ary[1] - g) > COLOR_THRESHOLD
      || abs(ary[2] - b) > COLOR_THRESHOLD) {
    return false;
  }
  return true;
}

void setLED(int red, int green, int blue) {
  analogWrite(redpin, red);
  analogWrite(greenpin, green);
  analogWrite(bluepin, blue);  
}

void processColor(int red, int green, int blue) {
  // writes the color to the LED based on the surface it's on
  // if it's on RED surface

  Color newColor;
  if (colorCompare(red, green, blue, RED_RGB)) {
    red = 255;
    green = 0;
    blue = 0;
    newColor = RED;
  }
  // GREEN surface
  else if (colorCompare(red, green, blue, GREEN_RGB)) {
    red = 0;
    green = 255;
    blue = 0;
    newColor = GREEN;
  }
  // BLUE surface
  else if (colorCompare(red, green, blue, BLUE_RGB)) {
    red = 0;
    green = 0;
    blue = 255;
    newColor = BLUE;
  }
  // else LED is off
  else {
    red = 0;
    green = 0;
    blue = 0;
    newColor = OTHER;
  }

  if (newColor != currentColor && newColor != OTHER) {
    currentColor = newColor;
  }
  setLED(red, green, blue);
}

void handleDriving(int throttle, int steering) {
  throttle -= 49;
  steering -= 49;

  int leftVal, rightVal;
  double throttleReduction;

  // still if close enough to zero
  if (abs(throttle) < THROTTLE_ZERO_THRESHOLD) {
    leftVal = STILL;
    rightVal = STILL;
  }
  // forward
  else if (throttle > 0) {
    // turn left forward
    if (steering <= 0) {
      // steering: max abs = 49; min abs = 0
      throttleReduction = mapFloat(steering, 0, -49, 1, 0); 
      leftVal = map(throttle*throttleReduction, 0, 50, LEFT_FORWARD_MIN, LEFT_FORWARD_MAX);
      rightVal = map(throttle, 0, 50, RIGHT_FORWARD_MIN, RIGHT_FORWARD_MAX);
    }
    // turn right forward
    else {
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
      throttleReduction = mapFloat(steering, 0, -49, 1, 0); 
      leftVal = map(throttle*throttleReduction, 0, -49, LEFT_BACKWARD_MIN, LEFT_BACKWARD_MAX);
      rightVal = map(throttle, 0, -49, RIGHT_BACKWARD_MIN, RIGHT_BACKWARD_MAX);
    }
    // turn right backward
    else {
      throttleReduction = mapFloat(steering, 1, 50, 1, 0); 
      leftVal = map(throttle, 0, -49, LEFT_BACKWARD_MIN, LEFT_BACKWARD_MAX);
      rightVal = map(throttle*throttleReduction, 0, -49, RIGHT_BACKWARD_MIN, RIGHT_BACKWARD_MAX);
    }
  }
  
  leftServo.write(leftVal);
  rightServo.write(rightVal);
  
  /*Serial.print("left = "); Serial.print(leftVal);
  Serial.print(" | right = "); Serial.print(rightVal);
  Serial.print(" | r = "); Serial.print(throttleReduction);
  Serial.print(" | steering = "); Serial.print(steering);
  Serial.print(" | throttle = "); Serial.println(throttle); */
}

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);
  delay(100);
  Serial.println("Color View Test!");

  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1); // halt!
  }
  
  // use these three pins to drive an LED
  pinMode(redpin, OUTPUT);
  pinMode(greenpin, OUTPUT);
  pinMode(bluepin, OUTPUT);

  leftServo.attach(leftServoPin);
  rightServo.attach(rightServoPin);
}


void loop() {

  if (phone.getButton() == 0) {
    ledOff = !ledOff;
  }

  int throttle = phone.getThrottle();
  int steering = phone.getSteering();
  handleDriving(throttle, steering);
  
  uint16_t clear, red, green, blue;
  
  delay(60);  // takes 50ms to read 
  tcs.getRawData(&red, &green, &blue, &clear);
  tcs.setInterrupt(ledOff);

  // Figure out some basic hex code for visualization
  uint32_t sum = clear;
  float r, g, b;
  r = red; r /= sum;
  g = green; g /= sum;
  b = blue; b /= sum;
  r *= 256; g *= 256; b *= 256;

  processColor((int)r, (int)g, (int)b);
  Serial.print((int)r ); Serial.print(" "); Serial.print((int)g);Serial.print(" ");  Serial.println((int)b );

}

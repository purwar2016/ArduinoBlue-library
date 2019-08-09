/*
* ArduinoBlue example code for controling red, green, and blue LEDs.
*/

#include <SoftwareSerial.h>
#include <ArduinoBlue.h>

const unsigned long BAUD_RATE = 9600;

const int RED_PIN = 6;
const int GREEN_PIN = 5;
const int BLUE_PIN = 3;

// The bluetooth tx and rx pins must be supported by software serial.
// Visit https://www.arduino.cc/en/Reference/SoftwareSerial for unsupported pins.
// Bluetooth TX -> Arduino D8
const int BLUETOOTH_TX_PIN = 8;
const int BLUETOOTH_RX_PIN = 7;
 
SoftwareSerial bluetooth(BLUETOOTH_TX_PIN, BLUETOOTH_RX_PIN);
ArduinoBlue phone(bluetooth); // pass reference of bluetooth object to ArduinoCommander.

int button, sliderId, sliderVal;
int redVal = 255;
int greenVal = 255;
int blueVal = 255;
bool redOn, greenOn, blueOn;

void setLED(int pin, bool isOn, int value) {
  if (isOn) {
    analogWrite(pin, value);
  }
  else {
    analogWrite(pin, 0);
  }
}

void setup() {
  // Start serial communciations.
  // The baud rate must be the same for both serials.
  Serial.begin(BAUD_RATE);
  bluetooth.begin(BAUD_RATE);

  // Declare LED pins as output pins
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  
  Serial.println("setup complete");
}

void loop() {
  button = phone.getButton();
  sliderId = phone.getSliderId();
  sliderVal = phone.getSliderVal();

  // Change the brightness value of LED based on slider
  // Slider values go from 0 to 200
  if (sliderId == 0) {
    redVal = map(sliderVal, 0, 200, 0, 255);
  }
  else if (sliderId == 1) {
    greenVal = map(sliderVal, 0, 200, 0, 255);
  }
  else if (sliderId == 2) {
    blueVal = map(sliderVal, 0, 200, 0, 255);
  }

  // Set the LEDs on/off based on button press
  if (button == 0) {
    redOn = !redOn;
  }
  else if (button == 1) {
    greenOn = !greenOn;
  }
  else if (button == 2) {
    blueOn = !blueOn;
  }
  
  setLED(RED_PIN, redOn, redVal);
  setLED(GREEN_PIN, greenOn, greenVal);
  setLED(BLUE_PIN, blueOn, blueVal);
}

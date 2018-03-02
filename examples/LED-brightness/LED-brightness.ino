/*
* Control the brightness of LED using a slider.
*/

#include <SoftwareSerial.h>
#include <ArduinoBlue.h>


// The bluetooth tx and rx pins must be supported by software serial.
// Visit https://www.arduino.cc/en/Reference/SoftwareSerial for unsupported pins.
// Bluetooth TX -> Arduino D8
const int BLUETOOTH_TX = 11;
// Bluetooth RX -> Arduino D7
const int BLUETOOTH_RX = 10;

// LED (+) -> Arduino D9
const int LED_PIN = 9;

SoftwareSerial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX);
ArduinoBlue phone(bluetooth); // pass reference of bluetooth object to ArduinoBlue.

bool isLedOn = false;

void setup() {
    // Start serial monitor at 9600 bps.
    Serial.begin(9600);

    // Start bluetooth serial at 9600 bps.
    bluetooth.begin(9600);

    // delay just in case bluetooth module needs time to "get ready".
    delay(100);

    Serial.println("setup complete");

    // Declare LED pin as an output pin.
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  // Get slider ID and it's value
  int sliderId = phone.getSliderId();
  int sliderVal = phone.getSliderVal();

  // Get the button ID
  int buttionId = phone.getButton();

  // The sliderId will not be -1 if a slider has been touched.
  if (sliderId != -1) {
    // Adjust LED brightness.
    int writeVal = map(sliderVal, 0, 200, 0, 255);
    analogWrite(LED_PIN, writeVal);
    Serial.print("ID: "); Serial.print(sliderId);
    Serial.print("\tWriteVal: "); Serial.print(writeVal);
    Serial.print("\tVal: "); Serial.println(sliderVal);
  }
}

/*
* Demo program to demonstrate the features of the app.
*/

#include <SoftwareSerial.h>
#include <ArduinoBlue.h>


// List of supported pins: https://www.arduino.cc/en/Reference/SoftwareSerial
// Bluetooth TX -> Arduino D8
const int BLUETOOTH_TX = 8;
// Bluetooth RX -> Arduino D7
const int BLUETOOTH_RX = 7;

// variables
int prevThrottle = 49, prevSteering = 49, // 49 is the still value for the throttle and steering
        throttle, steering, prevMillis, sliderVal, button, sliderId;

SoftwareSerial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX);
ArduinoBlue phone(bluetooth); // pass reference of bluetooth object to ArduinoCommander.

// Setup code runs once after program starts
void setup() {
    // Start serial monitor at 9600 bps
    Serial.begin(9600);

    // Start bluetooth serial at 9600 bps
    bluetooth.begin(9600);

    // delay just in case bluetooth module needs time to "get ready"
    delay(100);

    Serial.println("setup complete");
    prevMillis = millis();
}

// Put your main code here, to run repeatedly:
void loop() {
    // ID of the button pressed pressed
    button = phone.getButton();

    // Returns the text data sent from the phone.
    // After it returns the latest data, empty string "" is sent in subsequent
    // calls until text data is sent again.
    String str = phone.getText();

    // throttle and steering values go from 0 to 99.
    throttle = phone.getThrottle();
    steering = phone.getSteering();

    // ID of the slider moved
    sliderId = phone.getSliderId();

    // slider value goes from 0 to 200
    sliderVal = phone.getSliderVal();

    // display button data whenever its pressed
    if (button != -1) {
        Serial.print("Button: ");
        Serial.println(button);
    }

    // display slider data when slider moves
    if (sliderId != -1) {
        Serial.print("Slider ID: ");
        Serial.print(sliderId);
        Serial.print("\tValue: ");
        Serial.println(sliderVal);
    }

    // display throttle and steering data if steering or throttle value is changed
    if (prevThrottle != throttle || prevSteering != steering) {
        Serial.print("Throttle: "); Serial.print(throttle); Serial.print(", Steering: "); Serial.println(steering);
        prevThrottle = throttle;
        prevSteering = steering;
    }

    if (str != "") {
        Serial.println(str);
    }

    // Send string from serial command line to the phone. This will alert the user.
    if (Serial.available()) {
        Serial.write("usb: ");
        String str = Serial.readString();
        phone.sendMessage(str);
        Serial.print(str);
        Serial.write('\n');
    }
}

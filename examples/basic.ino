/*
 * Prints the throttle, steering, button, and slider values to Serial output.
 * Sends serial monitor input as a popup on the phone.
 */

#include <SoftwareSerial.h>
#include <ArduinoCommander.h>


// PINS
#define BLUETOOTH_TX 8 // Arduino digital pin -> HM 10 TX
#define BLUETOOTH_RX 7 // Arduino digital pin -> HM 10 RX


// variables
int throttle, steering, prevMillis, sliderVal;
char button, sliderId;

SoftwareSerial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX);
ArduinoCommander phone(bluetooth); // pass reference of bluetooth object to ArduinoCommander.

void setup() {

    // begin serial monitor at 9600 bps (optional)
    Serial.begin(9600);

    // Start bluetooth serial at 9600 bps
    bluetooth.begin(9600);

    delay(100);

    Serial.println("setup complete");

    while (!phone.isConnected()) {
        // while the bluetooth module is not connected to the App.
        // Note that the bluetooth module is connected to something when the red LED is on and not blinking,
        // but may not be connected to the app.
        Serial.println("Waiting for connection");
        delay(2000);
    }

}

void loop() {

    // this button value is the ASCII character you press on the Command page of the app.
    button = phone.getButton();

    // Returns the text data sent from the phone.
    // After it returns the latest data,
    // empty string "" is sent in subsequent calls until text data is sent again.
    String str = phone.getText();

    // throttle and steering values go from 0 to 99.
    throttle = phone.getThrottle();
    steering = phone.getSteering();

    // slider ID is an ASCII character. Returns null if there was no slider update.
    sliderId = phone.getSliderId();

    // slider value goes from 0 to 200
    sliderVal = phone.getSliderVal();



    if (button) {
        // display button data whenever its pressed
        // if button is not NULL
        Serial.print("Button: ");
        Serial.println(button);
    }

    if (sliderId) {
        // if sliderId is not NULL
        // display slider data when slider moves
        Serial.print("Slider ID: ");
        Serial.print(sliderId);
        Serial.print("\tValue: ");
        Serial.println(sliderVal);
    }

    if (abs(millis() - prevMillis) > 1000) {
        // display steering data every half second

        Serial.println("----------------------------");
        Serial.print("Throttle: ");
        Serial.println(throttle);
        Serial.print("Steering: ");
        Serial.println(steering);

        prevMillis = millis();
    }

    if (str != "") {
        Serial.print(str);
        Serial.print("\n");
    }


    // Send string from serial command line to the phone. This will alert the user.
    if (Serial.available()) {
        Serial.write("usb: ");
        String str = Serial.readString();
        phone.sendMsg(str);
        Serial.print(str);
        Serial.write('\n');
    }

}

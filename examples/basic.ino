/*
* Prints the throttle, steering, button, and slider values to Serial output.
*/

#include <SoftwareSerial.h>
#include <ArduinoCommander.h>


// PINS
const int BLUETOOTH_TX = 8;
const int BLUETOOTH_RX = 7;

// variables
int throttle, steering, prevMillis, sliderVal;
char button, sliderId;

SoftwareSerial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX);
ArduinoCommander phone(bluetooth); // pass reference of bluetooth object to ArduinoCommander.

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);

    // Start bluetooth serial at 9600 bps
    bluetooth.begin(9600);

    delay(100);

    Serial.println("setup complete");

    prevMillis = millis();
}

void loop() {
    // put your main code here, to run repeatedly:

    // check for any incoming bluetooth signal
    phone.checkBluetooth();

    // this button value is the ASCII character you press on the Command page of the app.
    button = phone.getButton();

    // Returns the text data sent from the phone. After it returns the latest data, empty string "" is sent in subsequent
    // calls until text data is sent again.
    String str = phone.getText();

    // throttle and steering values go from 0 to 99.
    throttle = phone.getThrottle();
    steering = phone.getSteering();

    // slider ID is an ASCII character
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

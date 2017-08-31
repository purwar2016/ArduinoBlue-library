/*
 * Change the position of the servo based on slider input of ID 'A'
 */

#include <SoftwareSerial.h>
#include <ArduinoCommander.h>
#include <Servo.h>


// PINS
#define BLUETOOTH_TX 8 // Arduino digital pin -> HM 10 TX
#define BLUETOOTH_RX 7 //Arduino digital pin -> HM 10 RX

// variables
int throttle, steering, prevMillis, sliderVal;
char button, sliderId;

SoftwareSerial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX);
ArduinoCommander phone(bluetooth); // pass reference of bluetooth object to ArduinoCommander.
Servo myservo;  // create servo object to control a servo

void setup() {

    Serial.begin(9600);

    // Start bluetooth serial at 9600 bps
    bluetooth.begin(9600);

    delay(100);
    Serial.println("setup complete");

    // attaches the servo on pin 9 to the servo object
    myservo.attach(9);
}

void loop() {

    // slider ID is an ASCII character
    sliderId = phone.getSliderId();

    // slider value goes from 0 to 200
    sliderVal = phone.getSliderVal();

    if (sliderId == 'A') {
        // display slider data when slider A moves
        Serial.print("Slider ID: ");
        Serial.print(sliderId);
        Serial.print("\tValue: ");
        Serial.println(sliderVal);

        int pos = map(sliderVal, 0, 200, 0, 180);
        myservo.write(pos);
    }
}

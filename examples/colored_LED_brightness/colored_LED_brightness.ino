/*
* Sample code to control the brightness of multiple LEDs
*/

#include <SoftwareSerial.h>
#include <ArduinoCommander.h>


// pins
const int BLUETOOTH_TX = 8;  // TX-O pin of bluetooth mate, Arduino D2
const int BLUETOOTH_RX = 7;  // RX-I pin of bluetooth mate, Arduino D3
const int LED_RED = 3;  // PWM pin required for brightness control
const int LED_GREEN = 10; // PWM pin required for brightness control
const int LED_BLUE = 11; // PWM pin required for brightness control

SoftwareSerial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX);
ArduinoCommander phone(bluetooth);



void setup() {
    // the setup function runs once when you press reset or power the board

    // Begin the serial monitor at 9600bps
    Serial.begin(9600);

    // Start bluetooth serial at 9600
    bluetooth.begin(9600);

    delay(100);

    // setup LEDs
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);

    Serial.println("setup complete");
}

void loop() {
    // the loop function runs over and over again until power down or reset

    // check for any incoming bluetooth signal
    phone.checkBluetooth();

    // slider ID is an ASCII character
    char sliderId = phone.getSliderId();

    // slider val goes from 0 to 200
    int sliderVal = phone.getSliderVal();


    if (sliderId) {
        // if there was slider movement

        int pwm = map(sliderVal, 0, 200, 0, 255);
        switch (sliderId) {
            case 'A':
                analogWrite(LED_RED, pwm);
                break;
            case 'B':
                analogWrite(LED_GREEN, pwm);
                break;
            case 'C':
                analogWrite(LED_BLUE, pwm);
                break;
        }
    }

}

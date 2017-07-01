/*
 * Sample code to switch on/off colored LEDs.
 * You can program the buttons to do whatever you like :)
 */

#include <SoftwareSerial.h>
#include <MobileBLE.h>


// pins
const int BLUETOOTH_TX = 8;
const int BLUETOOTH_RX = 7;
const int LED_RED = 2;
const int LED_GREEN = 3;
const int LED_BLUE = 4;

SoftwareSerial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX);
MobileBLE phone(bluetooth);

void buttonSwitch(int pin) {
    // LED switch.

    if (digitalRead(pin) == HIGH) {
        digitalWrite(pin, LOW);
    }
    else {
        digitalWrite(pin, HIGH);
    }
}

void setup() {
    // the setup function runs once when you press reset or power the board

    // Begin the serial monitor at 9600bps
    Serial.begin(9600);

    // Start bluetooth serial at 9600
    bluetooth.begin(9600);

    // setup LEDs
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);

    Serial.println("setup complete");
}

void loop() {
    // the loop function runs over and over again until power down or reset

    // check for incoming signal
    phone.checkBluetooth();

    // gets the latest button press if available. Once this is called calling it again without any new button presses
    // will return NULL, which in layman's terms means nothing.
    char button = phone.getButton();

    // handle any button press
    if (button) {
        // if button is not NULL
        switch (button) {
            case 'A':
                buttonSwitch(LED_RED);
                break;
            case 'B':
                buttonSwitch(LED_GREEN);
                break;
            case 'C':
                buttonSwitch(LED_BLUE);
                break;
        }
    }

}

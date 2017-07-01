/*
 * Prints the throttle, steering, and button values to Serial output.
 */

#include <SoftwareSerial.h>
#include <MobileBLE.h>


// PINS
const int BLUETOOTH_TX = 8;
const int BLUETOOTH_RX = 7;

// variables
int throttle, steering, prevMillis;
char button;

SoftwareSerial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX);
MobileBLE phone(bluetooth); // pass reference of bluetooth object to MobileBLE.

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

    // throttle and steering values go from 0 to 99.
    throttle = phone.getThrottle();
    steering = phone.getSteering();

    if (button) {
        // display button data whenever its pressed
        // if button is not NULL
        Serial.print("Button: ");
        Serial.println(button);
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

}

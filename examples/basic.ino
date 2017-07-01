/*
 * Prints the throttle, steering, and button values to Serial output.
 */

#include <SoftwareSerial.h>
#include <MobileBLE.h>


// PINS
const int BLUETOOTH_TX = 8;
const int BLUETOOTH_RX = 7;

// variables
int throttle, steering;
char button;

SoftwareSerial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX);
MobileBLE phone(bluetooth); // pass reference of bluetooth object to MobileBLE.

void setup() {
    // put your setup code here, to run once:

    // Start bluetooth serial at 9600 bps
    bluetooth.begin(9600);

    delay(100);

    Serial.println("setup complete");
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
    button = phone.getButton();

    Serial.println("----------------------------");
    Serial.print("Throttle: ");
    Serial.println(throttle);
    Serial.print("Steering: ");
    Serial.println(steering);
    Serial.print("Button: ");
    Serial.println(button);

}

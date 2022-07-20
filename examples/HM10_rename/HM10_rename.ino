/*
 * Renames the HM 10 bluetooth module
 */

#include <SoftwareSerial.h>
#include <ArduinoBlue.h>

const unsigned long BAUD_RATE = 9600;

// PINS
const int BLUETOOTH_TX = 7;
const int BLUETOOTH_RX = 8;

SoftwareSerial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX);
ArduinoBlue phone(bluetooth); // pass reference of bluetooth object to ArduinoBlue constructor.

void setup() {

    // Start bluetooth serial
    bluetooth.begin(BAUD_RATE);

    delay(1000);

    // rename device to xxxxxx
    bluetooth.write("AT+NAMExxxxxx");

}

void loop() {

}

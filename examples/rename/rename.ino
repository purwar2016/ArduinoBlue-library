/*
 * Renames the HM 10 bluetooth module
 */

#include <SoftwareSerial.h>
#include <ArduinoBlue.h>


// PINS
const int BLUETOOTH_TX = 8;
const int BLUETOOTH_RX = 7;

SoftwareSerial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX);
ArduinoBlue phone(bluetooth);

void setup() {

    // Start bluetooth serial at 9600 bps
    bluetooth.begin(9600);

    delay(1000);

    // rename device to xxxxxx
    bluetooth.write("AT+NAMExxxxxx");

}

void loop() {

}
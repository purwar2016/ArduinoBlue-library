/*
 * Sample code to control the brightness of multiple LED.
 */

#include <SoftwareSerial.h>
#include <ArduinoCommander.h>


// pins
#define BLUETOOTH_TX 8  // Arduino digital pin -> HM 10 TX
#define BLUETOOTH_RX 7  // Arduino digital pin -> HM 10 RX
#define LED_RED      3  // PWM pin required for brightness control
#define LED_GREEN    10 // PWM pin required for brightness control
#define LED_BLUE     11 // PWM pin required for brightness control

SoftwareSerial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX);
ArduinoCommander phone(bluetooth);



void setup() {
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

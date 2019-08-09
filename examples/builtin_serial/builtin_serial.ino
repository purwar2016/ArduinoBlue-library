/*
Uses built-in hardware serial instead of software serial.
Recommended for Arduino boards with more than one serial ports.
https://www.arduino.cc/reference/en/language/functions/communication/serial/
*/

#include <SoftwareSerial.h>
#include <ArduinoBlue.h>

const unsigned long BAUD_RATE = 9600;

int prevThrottle = 49;
int prevSteering = 49;
int throttle, steering, sliderVal, button, sliderId;

// See: https://www.arduino.cc/reference/en/language/functions/communication/serial/
ArduinoBlue phone(Serial2);

// Setup code runs once after program starts.
void setup() {
    // Start serial communications.
    // The baud rate must be the same for both serials.
    Serial.begin(BAUD_RATE);
    Serial2.begin(BAUD_RATE);
    
    delay(100);

    Serial.println("setup complete");
}

// Put your main code here, to run repeatedly:
void loop() {
    // ID of the button pressed pressed.
    button = phone.getButton();

    // Returns the text data sent from the phone.
    // After it returns the latest data, empty string "" is sent in subsequent.
    // calls until text data is sent again.
    String str = phone.getText();

    // Throttle and steering values go from 0 to 99.
    // When throttle and steering values are at 99/2 = 49, the joystick is at center.
    throttle = phone.getThrottle();
    steering = phone.getSteering();

    // ID of the slider moved.
    sliderId = phone.getSliderId();

    // Slider value goes from 0 to 200.
    sliderVal = phone.getSliderVal();

    // Display button data whenever its pressed.
    if (button != -1) {
        Serial.print("Button: ");
        Serial.println(button);
    }

    // Display slider data when slider moves
    if (sliderId != -1) {
        Serial.print("Slider ID: ");
        Serial.print(sliderId);
        Serial.print("\tValue: ");
        Serial.println(sliderVal);
    }

    // Display throttle and steering data if steering or throttle value is changed
    if (prevThrottle != throttle || prevSteering != steering) {
        Serial.print("Throttle: "); Serial.print(throttle); Serial.print("\tSteering: "); Serial.println(steering);
        prevThrottle = throttle;
        prevSteering = steering;
    }

    // If a text from the phone was sent print it to the serial monitor
    if (str != "") {
        Serial.println(str);
    }

    // Send string from serial command line to the phone. This will alert the user.
    if (Serial.available()) {
        Serial.write("send: ");
        String str = Serial.readString();
        phone.sendMessage(str); // phone.sendMessage(str) sends the text to the phone.
        Serial.print(str);
        Serial.write('\n');
    }
}

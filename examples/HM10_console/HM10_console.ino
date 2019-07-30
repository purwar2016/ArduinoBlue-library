
#include <SoftwareSerial.h>

const unsigned long BAUD_RATE = 9600;

// The bluetooth tx and rx pins must be supported by software serial.
// Visit https://www.arduino.cc/en/Reference/SoftwareSerial for unsupported pins.
// Bluetooth TX -> Arduino D8
const int BLUETOOTH_TX = 8;
// Bluetooth RX -> Arduino D7
const int BLUETOOTH_RX = 7;

const int READ_TIME = 500;

SoftwareSerial bt(BLUETOOTH_TX, BLUETOOTH_RX);

void setup() {
	// Baud rate must be same for bluetooth and serial.
	// It helps to prevent interference.
	bt.begin(BAUD_RATE);
	Serial.begin(BAUD_RATE);
	Serial.println("Setup Complete");
}

void loop() {
	// while incoming signal is available.
	if (bt.available()) {
		unsigned long readStart = millis();
		while (bt.available() || millis() - readStart < READ_TIME) {
			if (bt.available()) {
				Serial.print((char)bt.read());
			}
		}
		Serial.println();
	}

	while (Serial.available()) {
		String input = Serial.readStringUntil('\r');
		String output = "";
		for (int i = 0; i < input.length(); i++) {
			char c = input.charAt(i);
			if (c != 10 && c != 13) { // Don't send line end characters to HM 10 
				output += c;
			}
		}
		Serial.print("Entered: "); Serial.println(output);
		bt.print(output);
	}
}

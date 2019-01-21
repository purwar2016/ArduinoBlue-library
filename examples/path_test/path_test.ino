/*
* Demo program to demonstrate the features of the app.
*/

#include <SoftwareSerial.h>
#include <ArduinoBlue.h>


// The bluetooth tx and rx pins must be supported by software serial.
// Visit https://www.arduino.cc/en/Reference/SoftwareSerial for unsupported pins.
// Bluetooth TX -> Arduino D8
const int BLUETOOTH_TX = 8;
// Bluetooth RX -> Arduino D7
const int BLUETOOTH_RX = 7;

int prevThrottle = 49;
int prevSteering = 49;
int throttle, steering, sliderVal, button, sliderId;

bool hasPathPrinted = false;

SoftwareSerial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX);
ArduinoBlue phone(bluetooth); // pass reference of bluetooth object to ArduinoBlue constructor

// Setup code runs once after program starts.
void setup() {
	// Start serial monitor at 9600 bps.
	Serial.begin(9600);

	// Start bluetooth serial at 9600 bps.
	bluetooth.begin(9600);

	// delay just in case bluetooth module needs time to "get ready".
	delay(100);

	Serial.println("setup complete");
}

void printCoordinate(float x, float y) {
	Serial.print(x); Serial.print("\t");
	Serial.println(y);
}

// Put your main code here, to run repeatedly:
void loop() {
	// ID of the button pressed pressed.
	button = phone.getButton();

	// Display button data whenever its pressed.
	if (button != -1) {
		Serial.print("Button: ");
		Serial.println(button);
	}

	if (phone.isPathAvailable() && !hasPathPrinted) {
		// GET THE PATH
		int length = phone.getPathLength();
		float * pathX = phone.getPathArrayX();
		float * pathY = phone.getPathArrayY();

		// PRINT RECEIVED PATH POINTS
		Serial.println("__________________ PRINTING PATH ____________________");
		for (int i = 0; i < length; i++) {
			printCoordinate(pathX[i], pathY[i]);
		}

		// PRINT INTERPOLATED POINTS
		// Interpolates points between X coordinates from path x array that are indexStep apart.
		const int indexStep = 3;
		// StepX defines the distance between each interpolation.
		const int stepX = 10;

		Serial.println("__________________ Interpolation Test ________________");
		int index = 0;
		while (index < length - indexStep) {
			int startX = pathX[index];
			int endX = pathX[index + indexStep];

			for (int x = startX; x < endX; x += stepX) {
				printCoordinate(x, phone.getPathY(x));
			}

			index += indexStep;
		}
		hasPathPrinted = true;
	}
}

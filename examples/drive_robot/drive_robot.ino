/*
ArduinoBlue-library demo for driving differential drive robot.
Instructions:
  1. Set the pin constants.
  2. Edit the motorSetForward, motorSetBackward, and motorBrake functions as necessary (see comments).
  3. Test the functions from step 2 by calling them and setting the motor speed to max (see doTesting function)
		CAUTION: The motor set functions must not set the motor speed. This function simply configures the motor controller.
  4. Uncomment controlDrive function and comment doTesting function in the loop
  5. Connect to app and drive.
 */


#include <SoftwareSerial.h> // SoftwareSerial for serial communication with HM10 bluetooth module.
#include <ArduinoBlue.h> // ArduinoBlue bluetooth library

const unsigned long BAUD_RATE = 9600;

// MOTOR LEFT PINS
const int ENA = 11;
const int IN1 = 25;
const int IN2 = 24;
// MOTOR RIGHT PINS
const int ENB = 12;
const int IN3 = 22;
const int IN4 = 23;


// MOTOR PARAMETERS
// Minimum PWM value of analogWrite to spin motor when robot is on the ground.
const int MINIMUM_MOTOR_SPEED = 65;


// HM10 BLUETOOTH MODULE PINS
// NOTE: Not all pins on your Arduino may support SoftwareSerial.
// Please check: https://www.arduino.cc/en/Reference/softwareSerial
const int BLUETOOTH_TX = 50;
const int BLUETOOTH_RX = 51;

SoftwareSerial softSerial(BLUETOOTH_TX, BLUETOOTH_RX); // Object for serial communication to HM 10 bluetooth module using ditigal pins.
ArduinoBlue phone(softSerial); // Object for smartphone robot control.

// Configures the motor controller to stop the motors.
void motorBrake() {
	digitalWrite(ENA, LOW);
	digitalWrite(ENB, LOW);
	digitalWrite(IN1, LOW);
	digitalWrite(IN2, LOW);
	digitalWrite(IN3, LOW);
	digitalWrite(IN4, LOW);
	digitalWrite(ENA, HIGH);
	digitalWrite(ENB, HIGH);
	// Do not write the motor speeds on this function. It simply configures the motor controller.
}

// Configures the motor controller to have the robot move forward.
void motorSetForward() {
	digitalWrite(IN1, LOW);
	digitalWrite(IN2, HIGH);
	digitalWrite(IN3, LOW);
	digitalWrite(IN4, HIGH);
	// Do not write the motor speeds on this function. It simply configures the motor controller.
}

// Configures the motor controller to have the robot move backwards.
void motorSetBackward() {
	digitalWrite(IN1, HIGH);
	digitalWrite(IN2, LOW);
	digitalWrite(IN3, HIGH);
	digitalWrite(IN4, LOW);
	// Do not write the motor speeds on this function. It simply configures the motor controller.
}

void setPins() {
	// Set pins as input or output
	pinMode(ENA, OUTPUT);
	pinMode(IN1, OUTPUT);
	pinMode(IN2, OUTPUT);
	pinMode(ENB, OUTPUT);
	pinMode(IN3, OUTPUT);
	pinMode(IN4, OUTPUT);
}

void controlDrive() {
	// THROTTLE AND STEERING CONTROL
	// throttle values after subtracting 49:
	//     50 = max forward throttle
	//     0 = no throttole
	//     -49 = max reverse throttle
	// steering values after subtracting 49:
	//     50 = max right
	//     0 = straight
	//     -49 = max left
	int throttle = phone.getThrottle() - 49;
	int steering = phone.getSteering() - 49;

	if (throttle == 0) {
		// If throttle is zero, don't move.
		motorBrake();
		return;
	}

	// Determine forwards or backwards.
	if (throttle > 0) {
		// Forward
		motorSetForward();
	}
	else {
		// Backward
		motorSetBackward();
	}

	// Map throttle to PWM range.
	int mappedSpeed = map(abs(throttle), 0, 50, MINIMUM_MOTOR_SPEED, 255);
	// Map steering to PWM range.
	int reducedSpeed = map(abs(steering), 0, 50, mappedSpeed, MINIMUM_MOTOR_SPEED);

	int leftMotorSpeed, rightMotorSpeed;
	if (steering > 0) {
		// Turn Right: reduce right motor speed
		leftMotorSpeed = mappedSpeed;
		rightMotorSpeed = reducedSpeed;
	}
	else {
		// Turn Left: reduce left motor speed
		leftMotorSpeed = reducedSpeed;
		rightMotorSpeed = mappedSpeed;
	}

	// Set motor speeds
	analogWrite(ENA, leftMotorSpeed);
	analogWrite(ENB, rightMotorSpeed);

	// Print Debug Info
	Serial.print("throttle: "); Serial.print(throttle);
	Serial.print("\tsteering: "); Serial.print(steering);
	Serial.print("\tmappedSpeed: "); Serial.print(mappedSpeed);
	Serial.print("\treducedSpeed: "); Serial.print(reducedSpeed);
	Serial.print("\tleftMotorSpeed: "); Serial.print(leftMotorSpeed);
	Serial.print("\trightMotorSpeed: "); Serial.println(rightMotorSpeed);
}

void doTesting() {
	// Uncomment to test.
	// motorSetForward();
	// motorSetBackward();
	// motorBrake();

	// Set motor speeds to max
	analogWrite(ENA, 255);
	analogWrite(ENB, 255);
}

void setup() {
	delay(500);
        // Start serial communications.
        // The baud rate must be the same for both serials.
	// Start communication with HM10 bluetooth module.
	softSerial.begin(BAUD_RATE);

	// Begin serial communication with computer.
	Serial.begin(BAUD_RATE);

	// Set pin modes
	setPins();

	Serial.println("SETUP COMPLETE");
}

void loop() {
	// Refer to comment on top for step by step instructions.
	doTesting(); // Comment this after testing.
	// controlDrive(); // Uncomment this after testing.
}

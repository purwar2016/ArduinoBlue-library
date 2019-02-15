/*
ArduinoBlue-library demo for driving differential drive robot.
Instructions:
  1. Set the pin constants.
  2. Connect with app and drive. If the robot goes backwards, set DIRECTION to -1.
 */


#include <SoftwareSerial.h> // SoftwareSerial for serial communication with HM10 bluetooth module.
#include <ArduinoBlue.h> // ArduinoBlue bluetooth library
#include <Servo.h>

// SERVO PINS
// Servo pins must be PWM comaptible.
const int SERVO_LEFT_PIN = 0;
const int SERVO_RIGHT_PIN = 0;


// HM10 BLUETOOTH MODULE PINS
// NOTE: Not all pins on your Arduino may support SoftwareSerial.
// Please check: https://www.arduino.cc/en/Reference/softwareSerial
const int BLUETOOTH_TX = 50;
const int BLUETOOTH_RX = 51;


SoftwareSerial softSerial(BLUETOOTH_TX, BLUETOOTH_RX); // Object for serial communication to HM 10 bluetooth module using ditigal pins.
ArduinoBlue phone(softSerial); // Object for smartphone robot control.


// Minimum continuous rotation servo speed.
const int MINIMUM_SERVO_SPEED = 0;
 // If forward is backwards, change this to -1.
const int DIRECTION = 1;

Servo servoLeft;
Servo servoRight;


void driveControl() {
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
		servoLeft.write(90);
		servoRight.write(90);
		return;
	}

	// Map throttle to PWM range.
	int mappedSpeed = map(abs(throttle), 0, 50, MINIMUM_SERVO_SPEED, 90);
	// Map steering to PWM range.
	int reducedSpeed = map(abs(steering), 0, 50, mappedSpeed, MINIMUM_SERVO_SPEED);

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
	if (throttle > 0) {
		// Forward
		servoLeft.write(90+leftMotorSpeed*DIRECTION);
		servoRight.write(90-rightMotorSpeed*DIRECTION);
	}
	else {
		// Backward
		servoLeft.write(90-leftMotorSpeed*DIRECTION);
		servoRight.write(90+rightMotorSpeed*DIRECTION);
	}

	// Print Debug Info
	//  Serial.print("throttle: "); Serial.print(throttle);
	//  Serial.print("\tsteering: "); Serial.print(steering);
	//  Serial.print("\tmappedSpeed: "); Serial.print(mappedSpeed);
	//  Serial.print("\treducedSpeed: "); Serial.print(reducedSpeed);
	//  Serial.print("\tleftMotorSpeed: "); Serial.print(leftMotorSpeed);
	//  Serial.print("\trightMotorSpeed: "); Serial.println(rightMotorSpeed);
}

void setup() {
	delay(500);

	// Start communication with HM10 bluetooth module.
	softSerial.begin(9600);

	// Begin serial communication with computer.
	Serial.begin(9600);

	// Attach servos
	servoLeft.attach(SERVO_LEFT_PIN);
	servoRight.attach(SERVO_RIGHT_PIN);

	Serial.println("SETUP COMPLETE");
}

void loop() {
	// Refer to comment on top for step by step instructions.
	driveControl();
}

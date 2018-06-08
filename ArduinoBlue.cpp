/*
Name: ArduinoBlue.cpp
Created: 6/28/2017 11:00:39 AM
Author: Jae An
Contact: jaean37@gmail.com
*/

#include "ArduinoBlue.h"
#include <Arduino.h>
#include <LinkedList.h>

typedef union {
	float number;
	uint8_t bytes[4];
} FLOATUNION_t;

ArduinoBlue::ArduinoBlue(Stream &output) :
        _bluetooth(output)
{
}

bool ArduinoBlue::checkBluetooth() {

    bool isDataRead = _bluetooth.available() > 0;

    while (_bluetooth.available() > 0) {
        uint8_t intRead = _bluetooth.read();

        // Check for transmission starting
        // If a new transmission starts process the transmission
        if (intRead == DRIVE_TRANSMISSION) {
            processDriveTransmission();
        }
        else if (intRead == BUTTON_TRANSMISSION) {
			Serial.println("BUTTON TRANSMISSION");
            processButtonTransmission();
        }
        else if (intRead == SLIDER_TRANSMISSION) {
            processSliderTransmission();
        }
        else if (intRead == TEXT_TRANSMISSION) {
            processTextTransmission();
        }
        else if (intRead == PATH_TRANSMISSION) {
			Serial.println("PROCESSING PATH TRANSMISSION");
            processPathTransmission();
        }
        else if (intRead == CONNECTION_CHECK) {
            _bluetooth.print(CONNECTION_CHECK);
        }
    }

    return isDataRead;
}

void ArduinoBlue::processDriveTransmission() {
    storeShortTransmission();
    _throttle = _signal[0];
    _steering = _signal[1];
    clearSignalArray();
}

void ArduinoBlue::processButtonTransmission() {
    storeShortTransmission();
    _button = _signal[0];
    clearSignalArray();
}

void ArduinoBlue::processSliderTransmission() {
    storeShortTransmission();
    _sliderId = _signal[0];
    _sliderVal = _signal[1];
    clearSignalArray();
}

void ArduinoBlue::processTextTransmission() {
    _text = readString();
    clearSignalArray();
}

// TODO: Implement this.
void ArduinoBlue::processPathTransmission() {
	storePathTransmission();
	_pathAvailable = true;
    clearSignalArray();
}

static float ArduinoBlue::bytesToFloat(uint8_t u1, uint8_t u2, uint8_t u3, uint8_t u4) {
	FLOATUNION_t floatUntion;
	floatUntion.bytes[0] = u1;
	floatUntion.bytes[1] = u2;
	floatUntion.bytes[2] = u3;
	floatUntion.bytes[3] = u4;
	return floatUntion.number;
}

void ArduinoBlue::storePathTransmission() {
	unsigned long prevMillis = millis();
	uint8_t intRead;
	const int SIZE = 8;
	float ary[SIZE];
	int numElem = 0;
	_path.clear();

	Serial.println("PRINTING PATH");
	while (millis() - prevMillis < PATH_TRANSMISSION_TIMEOUT) {
		if (_bluetooth.available()) {
			intRead = _bluetooth.read();
			ary[numElem] = intRead;
			numElem++;
			Serial.print("numE: "); Serial.println(numElem);
			if (numElem == SIZE) {
				double n1 = bytesToFloat(ary[0], ary[1], ary[2], ary[3]);
				double n2 = bytesToFloat(ary[4], ary[5], ary[6], ary[7]);
				Serial.print("x: "); Serial.print(n1);
				Serial.print("\ty: "); Serial.println(n2);
				PathPoint *p = new PathPoint();
				p->x = n1;
				p->y = n2;
				_path.add(p);
				numElem = 0;
			}
			//if (intRead == TRANSMISSION_END) break;
		}
	}


	/*double x, y;
	for (int i = 0; i < 5; i++) {
		PathPoint *p = new PathPoint();
		x = i;
		y = i + 1;
		p->x = x;
		p->y = y;
		_path.add(p);
	} */
}

// Stores short transmission into the signal array
void ArduinoBlue::storeShortTransmission() {
	unsigned long prevMillis = millis();
	uint8_t intRead;
	while (millis() - prevMillis < SHORT_TRANSMISSION_TIMEOUT) {
		if (_bluetooth.available()) {
			intRead = _bluetooth.read();
			if (intRead == TRANSMISSION_END) break;
			pushToSignalArray(intRead);
		}
	}
}

String ArduinoBlue::readString() {
    String s;
    uint8_t intRead;
    unsigned long prevTime = millis();

    // Read until newline character or timeout is reached
    prevTime = millis();
    while (millis() - prevTime < TEXT_TRANSMISSION_TIMEOUT) {
        if (_bluetooth.available()) {
            intRead = _bluetooth.read();
            // break the loop if end of transmission
            if (intRead == TRANSMISSION_END) break;
            s.concat((char)intRead);
        }
    }
    return s;
}

void ArduinoBlue::pushToSignalArray(uint8_t elem) {
    if (elem < 0) {
        Serial.print("neg");
    }
    if ( !(_signalLength + 1 == MAX_SHORT_SIGNAL_LENGTH) ) {
        _signal[_signalLength] = elem;
        _signalLength++;
    }
    else {
        Serial.println("ArduinoBlue: Transmission error...");
    }
}

void ArduinoBlue::clearSignalArray() {
    for (uint8_t i = 0; i < _signalLength; i++) {
        _signal[i] = DEFAULT_VALUE;
    }
    _signalLength = 0;
}

bool ArduinoBlue::isPathAvailable() {
	bool toReturn = _pathAvailable;
	_pathAvailable = false;
	return toReturn;
}

int ArduinoBlue::getButton() {
    checkBluetooth();
    uint8_t btn = _button;
    _button = DEFAULT_VALUE;
    if (btn == DEFAULT_VALUE) return -1;
    return btn;
}

int ArduinoBlue::getSliderId() {
    checkBluetooth();
    uint8_t id = _sliderId;
    _sliderId = DEFAULT_VALUE;
    if (id == DEFAULT_VALUE) return -1;
    return id;
}

int ArduinoBlue::getSliderVal() {
    uint8_t val = _sliderVal;
    _sliderVal = DEFAULT_VALUE;
    if (val == DEFAULT_VALUE) return -1;
    return val;
}

int ArduinoBlue::getThrottle() {
    checkBluetooth();
    return _throttle;
}

int ArduinoBlue::getSteering() {
    checkBluetooth();
    return _steering;
}

LinkedList<PathPoint*> * ArduinoBlue::getPath() {
	return &_path;
}

void ArduinoBlue::sendText(String msg) {
    _bluetooth.print(((char)TEXT_SEND_TRANSMISSION) + msg + ((char)TRANSMISSION_END));
}

// for backwards compatibility
void ArduinoBlue::sendMessage(String msg) {
    sendText(msg);
}

bool ArduinoBlue::isConnected() {
    _bluetooth.print(CONNECTION_CHECK);
    // wait for 500 ms
    delay(500);
    if (_bluetooth.available()) {
        return _bluetooth.read() == CONNECTION_CHECK;
    }
    return false;
}

String ArduinoBlue::getText() {
    checkBluetooth();
    String ret = _text;
    _text = "";
    return ret;
}

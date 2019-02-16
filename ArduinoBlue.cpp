/*
Name: ArduinoBlue.cpp
Created: 6/28/2017 11:00:39 AM
Author: Jae An
Contact: jaean37@gmail.com
*/

#include "ArduinoBlue.h"
#include <Arduino.h>

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
            processButtonTransmission();
        }
        else if (intRead == SLIDER_TRANSMISSION) {
            processSliderTransmission();
        }
        else if (intRead == TEXT_TRANSMISSION) {
            processTextTransmission();
        }
        else if (intRead == PATH_TRANSMISSION) {
            processPathTransmission();
        }
        else if (intRead == CONNECTION_CHECK) {
            _bluetooth.print(CONNECTION_CHECK);
        }
    }

    return isDataRead;
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

void ArduinoBlue::processPathTransmission() {
    clearSignalArray();
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

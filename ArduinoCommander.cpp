/*
Name: ArduinoCommander.cpp
Created: 6/28/2017 11:00:39 AM
Author: Jae An
Contact: jaean37@gmail.com
*/

#include "ArduinoCommander.h"
#include <Arduino.h>

#define START_STR_TRANSMISSION 252
#define END_ORIENTATION_TRANSMISSION 254
#define END_BUTTON_TRANMISSION 255
#define END_SLIDER_TRANMISSION 253
#define STR_READ_FINISH_FEEDBACK "¶"

ArduinoCommander::ArduinoCommander(Stream &output) :
        _bluetooth(output)
{
}

bool ArduinoCommander::checkBluetooth() {

    bool dataRead = _bluetooth.available() > 0;

    while (_bluetooth.available() > 0) {

        // while bluetooth singal is available

        int input = _bluetooth.read();

        if (input == START_STR_TRANSMISSION) {
            // Start of transmission for text data
            _text = _bluetooth.readString();

            _bluetooth.print(STR_READ_FINISH_FEEDBACK);

            deleteElements();
        }

        push(input);

        if (input == END_ORIENTATION_TRANSMISSION) {
            // end of transmission for orientation
            _throttle = _signal[0];
            _steering = _signal[1];
            deleteElements(); // delete all elements in the singal array
        }
        else if (input == END_BUTTON_TRANMISSION) {
            // end of transmission for button click
            _button = (char)_signal[0];
            deleteElements();
        }
        else if (input == END_SLIDER_TRANMISSION) {
            // end of transmission for slider data
            _sliderId = (char)_signal[0];
            _sliderVal = _signal[1];
            deleteElements();
        }
    }

    return dataRead;

}

void ArduinoCommander::push(int elem) {
    _signal[_signalLength] = elem;
    _signalLength++;
}

void ArduinoCommander::deleteElements() {
    for (int i = 0; i < _signalLength; i++) {
        _signal[i] = NULL;
    }
    _signalLength = 0;
}

char ArduinoCommander::getButton() {
    checkBluetooth();
    char btn = _button;
    _button = NULL;
    return btn;
}

char ArduinoCommander::getSliderId() {
    checkBluetooth();
    char id = _sliderId;
    _sliderId = NULL;
    return id;
}

int ArduinoCommander::getSliderVal() {
    int val = _sliderVal;
    _sliderVal = NULL;
    return val;
}

int ArduinoCommander::getThrottle() {
    checkBluetooth();
    return _throttle;
}

int ArduinoCommander::getSteering() {
    checkBluetooth();
    return _steering;
}

void ArduinoCommander::sendMsg(String msg) {
    _bluetooth.print(msg);
}

bool ArduinoCommander::isConnected() {

    while (_bluetooth.available() > 0) {
        _bluetooth.read();
    }

    _bluetooth.print("~");

    unsigned int prevMillis = millis();

    // check for incoming signal for 1 seconds
    while (_bluetooth.available() <= 0) {
        if (millis() - prevMillis > 1000) {
            return false;
        }
    }

    if ((char)_bluetooth.read() == '~') {
        // if the first reading is the : symbol then it's connected to the app.
        return true;
    }

    // if not then it's not connected to ArduinoCommander app.
    return false;

}

String ArduinoCommander::getText() {
    checkBluetooth();
    String ret = _text;
    _text = "";
    return ret;
}
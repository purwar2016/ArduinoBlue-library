/*
Name: ArduinoCommander.cpp
Created: 6/28/2017 11:00:39 AM
Author: Jae An
Contact: jaean37@gmail.com
*/

#include "ArduinoCommander.h"
#include <Arduino.h>

ArduinoCommander::ArduinoCommander(Stream &output) :
        _bluetooth(output)
{

}

bool ArduinoCommander::checkBluetooth() {

    bool dataRead = _bluetooth.available() > 0;

    while (_bluetooth.available() > 0) {

        // while bluetooth singal is available

        int input = _bluetooth.read();

        // I know nasty fix, but there is a very strange bug that causes it to not work if
        // push(input) is called and this is called. It's most likely something to do with timing.
        // I'm using an integer array for driving, button, and slider data because Arduino is slow with strings.
        if (input == 252) {
            // START of transmission for text data
            _text = _bluetooth.readString();

            _bluetooth.print("¶");

            deleteElements();
        }

        push(input);

        if (input == 254) {
            // end of transmission for orientation
            _throttle = _signal[0];
            _steering = _signal[1];
            deleteElements(); // delete all elements in the singal array
        }
        else if (input == 255) {
            // end of transmission for button click
            _button = (char)_signal[0];
            deleteElements();
        }
        else if (input == 253) {
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

String ArduinoCommander::getText() {
    checkBluetooth();
    String ret = _text;
    _text = "";
    return ret;
}
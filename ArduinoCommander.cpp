/*
Name: ArduinoCommander.cpp
Created: 6/28/2017 11:00:39 AM
Author: Jae An
Contact: jaean37@gmail.com
*/

#include "ArduinoCommander.h"
#include <Arduino.h>

#define START_STR_TRANSMISSION 252
#define START_MOVE_COMMANDS_TRANSMISSION 251
#define END_MOVE_COMMANDS_TRANSMISSION 250
#define END_ORIENTATION_TRANSMISSION 254
#define END_BUTTON_TRANMISSION 255
#define END_SLIDER_TRANMISSION 253

#define STR_READ_FINISH_FEEDBACK "¶"
// TODO CHANGE THESE
#define MOVEMENT_READ_FINISH_FEEDBACK "¶"
#define MOVEMENT_DONE_FEEDBACK "¶"

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
        if (input == START_STR_TRANSMISSION) {
            // Start of transmission for text data
            _text = _bluetooth.readString();

            _bluetooth.print(STR_READ_FINISH_FEEDBACK);

            deleteElements();
        }

        if (input == START_MOVE_COMMANDS_TRANSMISSION) {
            // Start of transmission for movement command array.
            _readMoveCommands();
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

void ArduinoCommander::_readMoveCommands() {

    /*while (_bluetooth.available() > 0) {
        for (int i = 0; i < 4; i++) {
            _moveCommands[count*4 + i] = (char)_bluetooth.read();
        }

        count++;

        if (count == 49) {
            _bluetooth.print(MOVEMENT_READ_FINISH_FEEDBACK);
            break;
        }
    }*/

    bool breakNestedLoop = false;

    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 4; j++) {
            int input = _bluetooth.read();
            if (input == END_MOVE_COMMANDS_TRANSMISSION) {
                breakNestedLoop = true;
            }
            else {
                _moveCommands[i * 4 + j] = (char)input;

            }
        }
        if (breakNestedLoop) {
            break;
        }
    }

    _pathAvailable = true;

}

bool ArduinoCommander::isPathAvailable() {
    checkBluetooth();
    return _pathAvailable;
}

char (&ArduinoCommander::getMoveCommands())[200] {
        _bluetooth.print(MOVEMENT_DONE_FEEDBACK);

        _pathAvailable = false;

        return _moveCommands;
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

    _bluetooth.print("~");

    unsigned int prevMillis = millis();

    // check for incoming signal for 4 seconds
    while (_bluetooth.available() <= 0) {
        if (abs(millis() - prevMillis) < 4000) {
            return false;
        }
    }

    if ((char)_bluetooth.read() == '~') {
        // if the first reading is the : symbol then it's connected to the app.
        return true;
    }

    // if not then perhaps it's connected to something else.
    return false;

}

String ArduinoCommander::getText() {
    checkBluetooth();
    String ret = _text;
    _text = "";
    return ret;
}
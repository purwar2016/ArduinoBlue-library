/*
Name: ArduinoBLE.cpp
Created: 6/28/2017 11:00:39 AM
Author: Jae An
Contact: jaean37@gmail.com
*/

#ifndef _ArduinoBLE_h
#define _ArduinoBLE_h
#include <Arduino.h>

const int DEFAULT_STEERING = 49;
const int DEFAULT_THROTTLE = 49;

class ArduinoBLE
{
public:
    ArduinoBLE(Stream &output);
    int getButton();
    int getSliderId();
    int getSliderVal();
    int getThrottle();
    int getSteering();
    bool checkBluetooth();
    bool isConnected();
    void push(int elem);
    void deleteElements();
    void sendMsg(String msg);
    String getText();
private:
    Stream &_bluetooth;
    int _signal[10];
    int _signalLength = 0;
    int _throttle = DEFAULT_STEERING;
    int _steering = DEFAULT_THROTTLE;
    int _sliderVal;
    int _sliderId;
    int _button;
    String _text;
};

#endif
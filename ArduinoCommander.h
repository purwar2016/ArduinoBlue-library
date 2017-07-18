/*
Name: ArduinoCommander.cpp
Created: 6/28/2017 11:00:39 AM
Author: Jae An
Contact: jaean37@gmail.com
*/

#ifndef _ArduinoCommander_h
#define _ArduinoCommander_h
#include <Arduino.h>

const int DEFAULT_STEERING = 49;
const int DEFAULT_THROTTLE = 49;

class ArduinoCommander
{
public:
    ArduinoCommander(Stream &output);
    void begin();
    char getButton();
    char getSliderId();
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
    char _sliderId;
    char _button;
    String _text;
};

#endif
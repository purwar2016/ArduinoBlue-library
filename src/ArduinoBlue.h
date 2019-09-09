/*
Author: Jae An
*/

#ifndef ArduinoBlue_h
#define ArduinoBlue_h

#include "FunctionType.h"
#include <Arduino.h>

// Denotes start of path transmission from mobile app.
#define PATH_TRANSMISSION 244
// Denotes that the path transmission was received successfully by the Arduino. (Sent from Arduino).
#define PATH_TRANSMISSION_CONFIRMATION 245

// Denotes start of location transmission from Arduino
#define LOCATION_TRANSMISSION_START 246

// Denotes start of text send transmission from Arduino
#define TEXT_SEND_TRANSMISSION 248

// Denotes start of steering and throttle value transmission from mobile app.
#define DRIVE_TRANSMISSION 251

// Denotes start of button transmission from mobile app.
#define BUTTON_TRANSMISSION 252

// Denotes start of slider transmission from mobile app.
#define SLIDER_TRANSMISSION 253

// Denotes start of text transmission from mobile app.
#define TEXT_TRANSMISSION 254

// Denotes that the text, button, or slider transmission has been received successfully by the Arduino. (Sent from Arduino).
#define TRANSMISSION_END 250

// Default value for signal array elements.
#define DEFAULT_VALUE 255

// Timeouts ensure that the app does not get stuck in an endless loop.
// This is done as a precaution. 
#define TEXT_TRANSMISSION_TIMEOUT 5000 // See readString().
#define SHORT_TRANSMISSION_TIMEOUT 500 // See storeShortTransmission().
#define PATH_TRANSMISSION_TIMEOUT 10000 // See storePathTransmission().

#define PATH_OVERFLOW_VALUE 1000000 // See storePathTransmission().

// #define CONNECTION_CHECK 249

// Default values for steering and throttle.
// 49 for both steering and throttle means that the joystick is on the center.
const uint8_t DEFAULT_STEERING = 49;
const uint8_t DEFAULT_THROTTLE = 49;

// Maximum length of signal array.
const uint8_t MAX_SHORT_SIGNAL_LENGTH = 3;

class ArduinoBlue
{
public:
    ArduinoBlue(Stream &output);
    int getButton();
    int getSliderId();
    int getSliderVal();
    int getThrottle();
    int getSteering();
	float * getPathArrayX();
	float * getPathArrayY();
	float getPathY(float);
	int getPathLength();
    bool checkBluetooth();
    bool isConnected();
	bool isPathAvailable();
    void sendText(String msg);
    void sendMessage(String msg);
	void sendLocation(float, float, float, float, float);
	static float bytesToFloat(uint8_t u1, uint8_t u2, uint8_t u3, uint8_t u4);
    String getText();
    void setInterruptToggle(functiontype attach, functiontype detach);
private:
    Stream & _bluetooth;
    uint8_t _signal[MAX_SHORT_SIGNAL_LENGTH];
    uint8_t _signalLength = 0;
    uint8_t _throttle = DEFAULT_STEERING;
    uint8_t _steering = DEFAULT_THROTTLE;
    uint8_t _sliderVal = DEFAULT_VALUE;
    uint8_t _sliderId = DEFAULT_VALUE;
    uint8_t _button = DEFAULT_VALUE;
	bool _pathAvailable = false;
    String _text;
	float * _pathX;
	float * _pathY;
	float _prevReturnXx;
	int _pathLength;
    void clearSignalArray();
    void pushToSignalArray(uint8_t elem);
    void storeShortTransmission();
	bool storePathTransmission();
    void processDriveTransmission();
    void processButtonTransmission();
    void processSliderTransmission();
    void processTextTransmission();
    void processPathTransmission();
	void sendFloatAsBytes(float);
    void attachInterrupts();
    void detachInterrupts();
    String readString();
    functiontype _attachInterrupts = nullptr;
    functiontype _detachInterrupts = nullptr;
};

#endif

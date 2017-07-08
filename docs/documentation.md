

#Arduino Controller App Docs
This application has been tested to work with the [HM 10 Bluetooth module](https://www.google.com/search?q=hm10+bluetooth+module+arduino&oq=hm10+bluetooth+module+arduino&aqs=chrome..69i57j0j35i39l2j0l2.4673j0j1&sourceid=chrome&ie=UTF-8#safe=active&q=hm+10+bluetooth+module+arduino).
It should work fine for other Bluetooth low energy modules that communicate using serial communication, but only the HM 10 has been tested.

The mobile app runs on Android or iOS devices that support Bluetooth low energy. **Please note that not all Android devices with Bluetooth support Bluetooth low energy.** Check the specifications of your phone.

##Mobile App

####Settings Page
* Scan for devices
	* Enable bluetooth on your phone before scanning
	* Lists devices you can connect to
	* Click on the device to connect
* Tilt control
	* When this is enabled, you can drive the robot by tilting the phone and your phone's orientation will change to horizontal.
* Knob zero threshold

####Command Page
The command page allows you to command the Arduino to do stuff by using the buttons or sliders on this page. You can customize the layout of the buttons and sliders.

* To add a button or slider click Edit -> click +
	* the character ID is a single [ASCII](http://www.asciitable.com/) character that is used to identify the command in the Arduino code. Choose a unique character for every button and slider. Note that a slider and a button can share a character ID if necessary.
* Reorder the elements in edit mode by dragging the three bars.

####Drive Page

* Touch control mode:  simply touch the screen and dragging will show the
* Tilt control mode: left box will indicate the throttle and the right box indicates the steering.

###Arduino Library
This app is designed to work with a custom Arduino library. To use this library simply...

###Methods
```.getThrottle()``` -  Returns the latest throttle value (integer from 0 to 99).
```.getSteering()``` - Returns the latest steering value (integer from 0 to 99).
```.getButton()``` - Returns the character ID of the latest button pressed. Note that once this function returns the latest button press, it will return NULL on subsequent calls until another button is pressed.
```.getSliderId()``` - Returns the character ID of the latest slider that moved. Once this function returns the latest character ID, it will return NULL on subsequent calls until a slider moves.
```.getSliderVal()``` - Returns the value of the latest slider that moved (integer from 0 to 200). Once this function returns the latest slider value, it will return NULL on subsequent calls until a slider moves.



```C++
#include <SoftwareSerial.h>
#include <MobileBLE.h>

// pins
const int BLUETOOTH_TX = 8;  // TX-O pin of bluetooth mate, Arduino D2
const int BLUETOOTH_RX = 7;  // RX-I pin of bluetooth mate, Arduino D3

SoftwareSerial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX);

// pass software serial a reference to the SoftwareSerial object.
MobileBLE phone(bluetooth);

void setup() {
    // the setup function runs once when you press reset or power the board

    // Begin the serial monitor at 9600bps
    Serial.begin(9600);

    // Start bluetooth serial at 9600
    bluetooth.begin(9600);
}

void loop() {
    // the loop function runs over and over again until power down or reset

    // check for incoming signal
    phone.checkBluetooth();

    // throttle and steering values goes from 0 to 99.

    // 0 throttle corresponds to maximum negative and 99 corresponds to maximum positive throttle.
    int throttle = phone.getThrottle();

    // 0 steering corresponds to maximum left and 99 corresponds to maximum right
    int steering = phone.getSteering();

}

```


> Written with [StackEdit](https://stackedit.io/).








# Dev Notes


http://blog.blecentral.com/2015/05/05/hm-10-peripheral/
default values for service_uuid and characteristic_uuid are defined in constants

https://developer.mozilla.org/en-US/docs/Web/JavaScript/Typed_arrays
arraybuffers are written to device




https://github.com/don/cordova-plugin-ble-central#typed-arrays
Bluetooth cordova plugin

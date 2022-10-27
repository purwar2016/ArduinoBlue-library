# ArduinoBlue for Micropython

This is a port of the ArduinoBlue sketch to
[Micropython](https://micropython.org/). It requires a Bluetooth-LE
enabled device like e.g. the ESP32. This port implements a
HM-10 compatible interface on the ESP32's own bluetooth. No
real HM-10 us this required.

From the Apps point of view the ESP32 then behaves like a HM-10
equipped Arduino and it's possible to control the ESP32 under
Micropython from the ArduinoBlue app.

A simple demo is included. To run the demo simply copy all three
Python files into the root file system of your ESP32 and run
arduinoBlueDemo.py.

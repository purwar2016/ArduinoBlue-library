#
# arduinoBlueDemo.py
#
# simple demo for the Micropython port of arduinoBlue
#

import time, math
from arduinoBlue import arduinoBlue

class arduinoBlueDemo(arduinoBlue):
    def cmd(self, type, id, arg = None):
        print("CMD", type, id, arg)

        if type == "BUTTON":
            self.say("Hello World!!\nYou pressed button: "+str(id))
    

if __name__ == "__main__":
    m = arduinoBlueDemo()
    print("ArduinoBlue running");

    # the center of the screen seems to be at 4200/-4500 at start
    center = ( 4200, -4500)

    h = 0
    while True:
        time.sleep_ms(100)

        # make the path indicator run a circle of 30cm radius around the center
        h += 3.1415/100     # 100 steps per full turn -> one full turn every 10 seconds
        m.pos(center[0]+300*math.sin(h), center[1]-300*math.cos(h), h, center[0], center[1])
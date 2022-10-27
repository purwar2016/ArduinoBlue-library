# arduinoBlue.py
#  
# This is a Micropython implementation of arduinoBlue.
#
# https://github.com/purwar2016/ArduinoBlue-library/wiki/Documentation
#

import struct
from hm10 import HM10

from micropython import const

class arduinoBlue(HM10):
    # Denotes start of path transmission from mobile app.
    PATH_TRANSMISSION = const(244)

    # Denotes that the path transmission was received successfully by the Arduino. (Sent from Arduino).
    PATH_TRANSMISSION_CONFIRMATION = const(245)

    # Denotes start of location transmission from Arduino
    LOCATION_TRANSMISSION_START = const(246)

    # Denotes start of text send transmission from Arduino
    TEXT_SEND_TRANSMISSION = const(248)

    # Denotes start of steering and throttle value transmission from mobile app.
    DRIVE_TRANSMISSION = const(251)

    # Denotes start of button transmission from mobile app.
    BUTTON_TRANSMISSION = const(252)

    # Denotes start of slider transmission from mobile app.
    SLIDER_TRANSMISSION = const(253)

    # Denotes start of text transmission from mobile app.
    TEXT_TRANSMISSION = const(254)

    # Denotes that the text, button, or slider transmission has been received successfully by the Arduino. (Sent from Arduino).
    TRANSMISSION_END = const(250)

    # Default value for signal array elements.
    DEFAULT_VALUE = const(255)

    def __init__(self, name="ESP32"):
        super().__init__(name)
        self.buffer = b""
        self.path_state = None    # not parsing a path

    def decode(self, s):
        return ''.join(map(chr, s))

    def process_path(self):
        if not self.buffer: return 

        # print("P", len(self.buffer), self.buffer)

        # process groups of four bytes as floats
        while len(self.buffer) >= 4:
            f = struct.unpack('<f', self.buffer[:4])[0]
            self.buffer = self.buffer[4:]

            if self.path_state < 0:
                self.path_state = 2*int(f+0.5)
            else:
                if self.path_state & 1:
                    # update y of last coordinate
                    self.path[-1] = ( self.path[-1][0], f )
                else:
                    # append a new coordinate
                    self.path.append( (f,0) )

                self.path_state -= 1

                # end of path reached?
                if self.path_state == 0:
                    # acknowledge reception
                    self.tx(bytes([arduinoBlue.PATH_TRANSMISSION_CONFIRMATION]))
                    self.path_state = None

                    # handle path
                    self.cmd("PATH", None, self.path)
                    self.path = None

                    return True

        return False

    def rx(self, data):
        self.buffer += data

        if self.path_state != None:
            if not self.process_path():
                return

        # drop everything that is not e proper message start
        while self.buffer and (int(self.buffer[0]) < arduinoBlue.PATH_TRANSMISSION or (int(self.buffer[0]) > arduinoBlue.TEXT_TRANSMISSION)):
            self.buffer = self.buffer[1:]

        # nothing to process?
        if not self.buffer: return

        # path is special as we won't wait for the end marker
        if int(self.buffer[0]) == arduinoBlue.PATH_TRANSMISSION:
            print("PATH")
            self.path = [ ]
            self.buffer = self.buffer[1:]
            self.path_state = -1           # start a new path
            if not self.process_path():   
                return                     # path not complete yet

        # check if there's already a message end in buffer
        while arduinoBlue.TRANSMISSION_END in self.buffer:
            msg, self.buffer = self.buffer.split(bytes([arduinoBlue.TRANSMISSION_END]), 1)

            command = int(msg[0])
            payload = msg[1:]

            if command == arduinoBlue.TEXT_TRANSMISSION:
                self.cmd("TEXT", None, self.decode(payload))
            elif command == arduinoBlue.BUTTON_TRANSMISSION:
                self.cmd("BUTTON", int(payload[0]))
            elif command == arduinoBlue.SLIDER_TRANSMISSION:
                self.cmd("SLIDER", int(payload[0]), int(payload[1]))
            elif command == arduinoBlue.DRIVE_TRANSMISSION:
                self.cmd("DRIVE", None, { "y": int(payload[0]), "x": int(payload[1]) } )
            else:
                print("unknown command", command)

    def cmd(self, type, id, arg = None):
        pass
        
    def say(self, msg):
        self.tx(bytes([arduinoBlue.TEXT_SEND_TRANSMISSION]) + msg.encode("latin1") + bytes([arduinoBlue.TRANSMISSION_END]))

    def pos(self, x, y, h, xg, yg):
        self.tx(bytes( [ arduinoBlue.LOCATION_TRANSMISSION_START ] ))
        self.tx(struct.pack("<fffff", x, y, h, xg, yg))


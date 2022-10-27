# hm10.py
#
# This is a Micropython implementation of the HM10 uart service
#

import bluetooth, struct, time
from micropython import const

_IRQ_CENTRAL_CONNECT = const(1)
_IRQ_CENTRAL_DISCONNECT = const(2)
_IRQ_GATTS_WRITE = const(3)

# HM-10 adapter compatible UUIDs, one characteristic only, R/W/N
_UART_UUID = bluetooth.UUID("0000FFE0-0000-1000-8000-00805F9B34FB")
_UART_TX = ( bluetooth.UUID("0000FFE1-0000-1000-8000-00805F9B34FB"), 0x1a )
_UART_SERVICE = ( _UART_UUID, (_UART_TX, ), )

class HM10:
    def __init__(self, name="ESP32"):
        self._ble = bluetooth.BLE()
        self._conn_handle = None
        self._ble.active(True)
        self._ble.irq(self._irq)
        ((self._handle_uart,),) = self._ble.gatts_register_services((_UART_SERVICE,))
        self._ble.gatts_set_buffer(self._handle_uart, 200, True)

        self._payload = bytes()
        self.adv_append(0x01, struct.pack("B", 0x06))
        self.adv_append(0x09, name)
        self.adv_append(0x07, bytes(_UART_UUID))

        self._ble.gap_advertise(100000, adv_data=self._payload)

        # build advertise payload
    def adv_append(self, adv_type, value):
        self._payload += struct.pack("BB", len(value) + 1, adv_type) + value
 
    def _irq(self, event, data):
        if event == _IRQ_CENTRAL_CONNECT:
            self._conn_handle,_,_ = data
        elif event == _IRQ_CENTRAL_DISCONNECT:
            self._conn_handle = None
            self._ble.gap_advertise(100000, adv_data=self._payload)
        elif event == _IRQ_GATTS_WRITE:
            conn_handle, value_handle = data
            self.rx(self._ble.gatts_read(value_handle))

    def rx(self, data):
        print("RX:", data)

    def tx(self, data):
        if self._conn_handle != None:
            while data:
                # BLE can send max 20 bytes per message
                chunk = data if len(data) <= 20 else data[:20]
                self._ble.gatts_notify(self._conn_handle, self._handle_uart, chunk)
                data = data[len(chunk):]
                time.sleep(0.01)          # sending too fast results in out of memory


if __name__ == "__main__":
    hm = HM10("ESPUART")

    while True:
        time.sleep_ms(100)

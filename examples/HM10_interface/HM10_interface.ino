#include <SoftwareSerial.h>

const int BLUETOOTH_TX = 8;
const int BLUETOOTH_RX = 7;

const int READ_TIME = 500; //ms

unsigned long prevMillis;

SoftwareSerial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  bluetooth.begin(9600);
  Serial.println("Setup Complete");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    String str = "";
    Serial.print("Input: ");
    
    prevMillis = millis();
    while (millis() - prevMillis < READ_TIME) {
      if (Serial.available()) {
        char c = Serial.read();
        if (c != 10 && c != 13) { // Don't send line end characters to HM10.
          str += c;
        }
      }
    }
    
    bluetooth.print(str);
    Serial.println(str);
  }

  if (bluetooth.available()) {
    String str = "";
    Serial.print("HM10: ");
    
    prevMillis = millis();
    while (millis() - prevMillis < READ_TIME) {
      if (bluetooth.available()) {
        str += (char) bluetooth.read();
      }
    }
    Serial.println(str);
  }
}

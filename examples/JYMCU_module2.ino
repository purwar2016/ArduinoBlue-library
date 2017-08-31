/*
USB(mega2560)-to-BLE sketch

Apploader project
http://www.apploader.info

Anton Smirnov
2015

Note:
HM-10 applies baud rate set in 'AT+BAUD' after switch off and on
*/

int led = 13;
int ledHIGH = 0;
int prevMillis;

long bauds[] = {
        // major
        9600, 57600, 115200,

        // others
        19200, 38400, 4800, 2400, 1200, 230400
};

bool detectBleBaudRate() {
    /*Serial.println("Detecting BLE baud rate:");
    for (int i = 0; i<(sizeof(bauds) / sizeof(long)); i++) {
        Serial.write("Checking ");
        long cur_baud = bauds[i];
        Serial.println(cur_baud, DEC);

        Serial1.begin(cur_baud);
        Serial1.write("AT");
        Serial1.flush();
        delay(50);
        String response = Serial1.readString();
        if (response == "OK") {
            Serial.println("Detected");
            return true;
        }
        else {
            Serial1.end();
        }
    }
    return false;*/
}

void setup() {
    // init
    Serial.begin(9600);  // USB (choose 115200 in terminal)

    Serial1.begin(9600);
    Serial1.write("AT");
    Serial1.flush();

    Serial.write("Ready, type AT commands\n\n");

    pinMode(led, OUTPUT);

    prevMillis = millis();
}
String command = "";
void loop() {
    // read from BLE (HM-10)
    /*if (Serial1.available()) {
        Serial.write("ble: ");
        String str = Serial1.readString();
        Serial.print(str);
        Serial.write('\n');
    } */

    if (Serial1.available() > 0) {
        while(Serial1.available() > 0) { // While there is more to be read, keep reading.
            // Serial1.read();
            Serial.println(Serial1.readString());
        }
        Serial.println();
        command = ""; // No repeats
    }

    // read from USB (Arduino Terminal)
    if (Serial.available()) {
        Serial.write("usb: ");
        String str = Serial.readString();
        Serial1.println(str);
        Serial.println(str);
    }
}
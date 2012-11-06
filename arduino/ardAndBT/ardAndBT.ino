#include <SoftwareSerial.h>

int bluetoothTx = 2;
int bluetoothRx = 3;

const int LED_PIN1 = 9;
const int LED_PIN2 = 10;
const int LED_PIN3 = 11;

int currReading = 0;
boolean toggle1 = true;
boolean toggle2 = true;
boolean toggle3 = true;

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

void setup() {
    Serial.begin(9600);
    
    bluetooth.begin(115200);
    bluetooth.print("$$$");
    delay(100);
    bluetooth.println("U,9600,N");
    bluetooth.begin(9600);
    
    pinMode(LED_PIN1, OUTPUT);
    pinMode(LED_PIN2, OUTPUT);
    pinMode(LED_PIN3, OUTPUT);
}

void loop() {
    //if (Serial.available()) {
    //    currReading = Serial.read();
    if (bluetooth.available())
    {
      Serial.println("bla");
        currReading = bluetooth.read();
        if (currReading == 1)
        {
          toggle1 = !toggle1;
        }
        else if (currReading == 2)
        {
          toggle2 = !toggle2;
        }
        else if (currReading == 3)
        {
          toggle3 = !toggle3;
        }
    }
    digitalWrite(LED_PIN1, toggle1);
    digitalWrite(LED_PIN2, toggle2);
    digitalWrite(LED_PIN3, toggle3);
}

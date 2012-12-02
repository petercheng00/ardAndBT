#include <SoftwareSerial.h>


int bluetoothTx = 2;
int bluetoothRx = 3;

int IRledPin =  10;
int signalLedPin = 11;

String currReading = "";
boolean toggle1 = true;
boolean toggle2 = true;
boolean toggle3 = true;

boolean debugLED = false;

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

void setup() {
    Serial.begin(9600);
    
    bluetooth.begin(115200);
    bluetooth.print("$$$");
    delay(100);
    bluetooth.println("U,9600,N");
    bluetooth.begin(9600);
    pinMode(IRledPin, OUTPUT);
    pinMode(signalLedPin, OUTPUT);
}

void loop() {
    if (bluetooth.available())
    {
        currReading= bluetooth.readStringUntil('\n');
        Serial.println(currReading);
        unsigned int currentNum = 0;   
        int i;
        for(i = 0; i < currReading.length(); i++) {
          
          if(currReading.charAt(i) >= '0' && currReading.charAt(i) <= '9') {
            currentNum = currentNum * 10 + (currReading.charAt(i)-'0');
          } else if(currReading.charAt(i) == ',') {
              break;
          } 
        }
        unsigned int signal[currentNum];
        unsigned int valuesSeen = 0;
        int j;
        currentNum = 0;
        for(j = i+1; j < currReading.length(); j++) {
          if(currReading.charAt(j) >= '0' && currReading.charAt(j) <= '9') {
        
            currentNum = currentNum * 10 + (currReading.charAt(j)-'0');
            
          } else if(currReading.charAt(j) == ',') {
            signal[valuesSeen] = currentNum;
            currentNum = 0;
            valuesSeen++;
          } 
        }
         signal[valuesSeen] = currentNum;
         sendSignal(signal, sizeof(signal)/sizeof(unsigned int));
    }
}

void sendSignal(unsigned int signal[], int size){
  //while(true){
  for(int i = 0; i < size; i++) {
    if(i % 2 == 0) {
        delayMicroseconds(signal[i]);
    } else {
      //Serial.println("pulsing for");
      pulseIR(signal[i]);
    }
    //Serial.println(signal[i]);
  }
  //}
}

// This procedure sends a 38KHz pulse to the IRledPin 
// for a certain # of microseconds. We'll use this whenever we need to send codes
void pulseIR(long microsecs) {
 
  // we'll count down from the number of microseconds we are told to wait
 
  cli();  // this turns off any background interrupts
 
  while (microsecs > 0) {
    // 38 kHz is about 13 microseconds high and 13 microseconds low
   digitalWrite(IRledPin, HIGH);  // this takes about 3 microseconds to happen
   delayMicroseconds(9);         // hang out for 10 microseconds
   digitalWrite(IRledPin, LOW);   // this also takes about 3 microseconds
   delayMicroseconds(9);         // hang out for 10 microseconds
 
   // so 26 microseconds altogether
   microsecs -= 26;
  }
  sei();  // this turns them back on
}

void flashSignal() {
  for (int i = 0; i < 4; ++i)
  {
    digitalWrite(signalLedPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(signalLedPin,LOW);
    delayMicroseconds(250);
  }
}

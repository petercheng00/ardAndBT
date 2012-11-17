const int LED_PIN1 = 11;
const int LED_PIN2 = 12;
const int LED_PIN3 = 13;

int IRledPin =  14;

String currReading = "";
boolean toggle1 = true;
boolean toggle2 = true;
boolean toggle3 = true;

boolean debugLED = false;

HardwareSerial bluetooth = HardwareSerial();

void setup() {
    Serial.begin(9600);
    
    bluetooth.begin(9600);
    bluetooth.print("$$$");
    delay(100);
    bluetooth.println("U,9600,N");
    bluetooth.begin(9600);
    pinMode(LED_PIN1, OUTPUT);
    pinMode(LED_PIN2, OUTPUT);
    pinMode(LED_PIN3, OUTPUT);
    pinMode(IRledPin, OUTPUT);
    digitalWrite(LED_PIN1, HIGH);
    digitalWrite(LED_PIN2, HIGH);
    digitalWrite(LED_PIN3, HIGH);
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
      //Serial.println("waiting for");]
      if(signal[i] > 16000) {
        delay(signal[i]/1000);
      } else {
        delayMicroseconds(signal[i]+20);
      }
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
   delayMicroseconds(14);         // hang out for 10 microseconds
   digitalWrite(IRledPin, LOW);   // this also takes about 3 microseconds
   delayMicroseconds(15);         // hang out for 10 microseconds
 
   // so 26 microseconds altogether
   microsecs -= 26;
  }
 
  sei();  // this turns them back on
}

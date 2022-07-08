#include <Arduino.h>

//Libraries for I/O expander
#include <PCF8574.h>
#include <Wire.h>



//Pin Setup control LED
#define controlLED1 32
#define controlLED2 33
#define controlLedSleep 12

//define wakeup pin
#define expanderInterrupt GPIO_NUM_14

//define interrupt pin on ESP32
#define espInterrupt 34


//create pcf8574 object
PCF8574 pcf20(0x20);

//button pins on I/O expander
#define rockerSwitch1 0
#define rockerSwitch2 1

//interrupt service routine
void readButtons();



void setup() {

  //Pin Setup control LED
  pinMode(controlLED1, OUTPUT);
  pinMode(controlLED2, OUTPUT);
  pinMode(controlLedSleep, OUTPUT);
  digitalWrite(controlLED1, LOW);
  digitalWrite(controlLED2, LOW);
  digitalWrite(controlLedSleep, LOW);

  //start pcf8574
  pcf20.begin(21, 22);

  //setuip wake up of ESP
  esp_sleep_enable_ext0_wakeup(expanderInterrupt, LOW);

  //set up interrupt in ESP to read I/O
  attachInterrupt(34, readButtons, LOW);

}

void loop() {
  
  //turn on LED to check that ESP is awake
  digitalWrite(controlLedSleep, HIGH);

  //start deep sleep
  if (pcf20.read(rockerSwitch1) == HIGH && pcf20.read(rockerSwitch2) == HIGH){
    digitalWrite(controlLED1, LOW);
    digitalWrite(controlLED2, LOW);
    digitalWrite(controlLedSleep, LOW);
    //esp_deep_sleep_start();
  }


}

void IRAM_ATTR readButtons(){

  digitalWrite(controlLedSleep, LOW);

  if (pcf20.read(rockerSwitch1) == LOW){
    digitalWrite(controlLED1, HIGH);
  }
  else{
    digitalWrite(controlLED1, LOW);
  }

  if (pcf20.read(rockerSwitch2) == LOW){
    digitalWrite(controlLED2, HIGH);
  }
  else{
    digitalWrite(controlLED2, LOW);
  }

  delay(100);

}
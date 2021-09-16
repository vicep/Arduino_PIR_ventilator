//
// PIR sensor tester
//
 
int ventilator = 10;                // choose the pin for the LED
int PIRsens = 5;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status
const unsigned long int cekej = 90L*1000L;      // cekej xx ms
unsigned long int posledniPohyb;

int svBalkon = 9;
String zprava = "";
/*
* Arduino Wireless Communication Tutorial
*       Example 1 - Receiver Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

const unsigned long int interval = 1500L;      // cekej xx ms
unsigned long int posledniZprava;

void setup() {
  pinMode(ventilator, OUTPUT);      // declare LED as output
  pinMode(PIRsens, INPUT);     // declare sensor as input
  digitalWrite(ventilator, HIGH); 
  Serial.begin(9600);

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_LOW); //MIN LOW MED HIGH
  radio.startListening();
  pinMode(svBalkon, OUTPUT);
  digitalWrite(svBalkon, HIGH);
}
 
void loop(){
    if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    zprava = text;
    //Serial.println(zprava);
    } else {
      zprava = "";
    }
    if (zprava.indexOf("funguje") > -1) {
      digitalWrite(svBalkon, LOW);
      Serial.println("svitim");
      posledniZprava = millis();
    } else if ((millis()-posledniZprava) >= interval) {
        digitalWrite(svBalkon, HIGH);
    }

    
  val = digitalRead(PIRsens);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
    digitalWrite(ventilator, LOW);  // turn LED ON
    if (pirState == LOW) {
      // we have just turned on
      Serial.println("Motion detected!");
      // We only want to print on the output change, not state
      pirState = HIGH;
      posledniPohyb = millis();
    }
  } else {
    //digitalWrite(ledPin, HIGH); // turn LED OFF
    if (pirState == HIGH){
      // we have just turned of
      Serial.println("Motion ended!");
      // We only want to print on the output change, not state
      pirState = LOW;
    }
  }
  if((millis()-posledniPohyb)>=cekej){
    digitalWrite(ventilator, HIGH);
  }
}

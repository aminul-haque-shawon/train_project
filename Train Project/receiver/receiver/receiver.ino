#include <SPI.h>
#include <LiquidCrystal.h>

#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 8);
int red = A2, yellow = A3, green = A4;
const byte address[6] = "00005";
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);

}
char signalCode[32];
void loop() {
  // put your main code here, to run repeatedly:

  if (radio.available()) {
    radio.read(&signalCode, sizeof(signalCode));
    Serial.print("Available:");
    Serial.println(signalCode);
    if (signalCode != 0) {
      digitalWrite(signalCode, HIGH);
    } else {
      digitalWrite(green, LOW);
      digitalWrite(red, LOW);
      digitalWrite(yellow, LOW);
    }
  }

}

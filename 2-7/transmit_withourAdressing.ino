// Feather9x_TX

#include <SPI.h>
#include <RH_RF95.h>
#include<LiquidCrystal.h>

// for feather32u4
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 7

int red = 10, yellow = 11, green = 12;
int button;
int signalCode = 0;

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 868.0

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);
int Contrast=0;
const int v0 = 50, rs = 60, en = 90, d4 = 100, d5 = 110, d6 = 120, d7 = 130;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup()
{
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);


  pinMode(red, INPUT);

  pinMode(yellow, INPUT);
  pinMode(green, INPUT);
  pinMode(v0,OUTPUT);
  signalCode = 0;
  analogWrite(v0, Contrast);

  lcd.begin(16, 2);
  lcd.clear();

  Serial.begin(115200);
//  while (!Serial) {
//    delay(1);
//  }

  delay(100);

  Serial.println("Feather LoRa TX Test!");

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    Serial.println("Uncomment '#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug info");
    while (1);
  }
  Serial.println("LoRa radio init OK!");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);

  rf95.setTxPower(23, false);
}

int16_t packetnum = 0;  // packet counter, we increment per xmission
void transmit() {

  char radiopacket[20] = "";
  bool red_s = digitalRead(red);
  bool yellow_s = digitalRead(yellow);
  bool green_s = digitalRead(green);

  if (red_s == HIGH ) {
    button = 2;
  }
  else if (yellow_s == HIGH) {
    button = 3;
  }
  else if (green_s == HIGH) {
    button = 4;
  }
Serial.println(button);

  if (button == 2) {
    signalCode = 2;
  }
  else if (button == 3) {
    signalCode = 3;
  }
  else if (button == 4) {
    signalCode = 4;
  }

  size_t len = strlen(radiopacket);

  snprintf(radiopacket + len, sizeof radiopacket - len, "%d", signalCode);
  rf95.send((uint8_t *)radiopacket, 20);

  Serial.println("Waiting for packet to complete...");
  delay(10);
  rf95.waitPacketSent();
  String signalCodeStr=String(signalCode);
  displayMessage("Sending "+signalCodeStr);
  Serial.println(signalCode);
}
void loop()
{
//  delay(1000); // Wait 1 second between transmits, could also 'sleep' here!
  Serial.println("Transmitting..."); // Send a message to rf95_server

  transmit();


}
void displayMessage(String message) {
  lcd.setCursor(0, 0);
  lcd.print("Hello World");
}

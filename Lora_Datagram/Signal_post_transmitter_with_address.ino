// WIP needs practical testing
// Signal post transmitter
// Transmitters start with sendToWait and address should be receiver


#include <RHReliableDatagram.h>
#include <RH_RF95.h>
#include <SPI.h>
#include <LiquidCrystal.h>
#include <Adafruit_SleepyDog.h>

// These addresses need further study
// till then keep address same in post and train module
#define POST_ADDRESS 1 //signal post
#define TRAIN_ADDRESS 2 //train 

#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 7

int redPin = 10, y1Pin = 11, y2Pin = 12, greenPin = 13;
int pot = A0;
const int timeout = 2000; // timeout for retries

RH_RF95 driver(RFM95_CS, RFM95_INT); // select 32u4

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram manager(driver, TRAIN_ADDRESS);

//const int rs = 90, en = 60, d4 = 130, d5 = 120, d6 = 110, d7 = 100;
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// This function converts our pinmodes to a string "RY1Y2G"
// ex: red and green = "1001"
String convertToArray () {

  int red = digitalRead(redPin);
  int y1 = digitalRead(y1Pin);
  int y2 = digitalRead(y2Pin);
  int green = digitalRead(greenPin);
  //Serial.print("Red: ");
  //Serial.println(red);
  //Serial.print("y1: ");
  //Serial.println(y1);
  //Serial.print("y2: ");
  //Serial.println(y2);
  //Serial.print("green: ");
  //Serial.println(green);
  String output = String("");
  output = output + red + y1 + y2 + green;
  return output;

}
// Function to display message on LCD
//void displayMessage(String message, int line) {
//  lcd.setCursor(0, line);
//  String spaces = String("            "); //to fill the spaces
//  lcd.print(message + spaces);
//}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) ; // Wait for serial port to be available
  if (!manager.init())
    Serial.println("init failed");
  //    displayMessage("init failed", 0);
  driver.setFrequency(868.0);
  driver.setTxPower(20, false);

  pinMode(redPin, INPUT);
  pinMode(y1Pin, INPUT);
  pinMode(y2Pin, INPUT);
  pinMode(greenPin, INPUT);
  pinMode(pot, INPUT);

//  lcd.begin(16, 2);
//  lcd.clear();
}

uint8_t signal_code[] = "0000";  // RY1Y2G
uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];

void loop() {
  // put your main code here, to run repeatedly:

  String _str = convertToArray ();  // the signal array as string "1001"
//  displayMessage(_str, 0); // show signal code in lcd
  _str.toCharArray(signal_code, sizeof(signal_code)); // convert to uint8_t

  //  displayMessage("RSSI" + String(driver.lastRssi()), 1);

  int power = map(analogRead(pot), 0, 1023, 2, 20); // maps pot 2-20
  driver.setTxPower(power, false);

  if (manager.sendtoWait(signal_code, sizeof(signal_code), TRAIN_ADDRESS))
  {
    // Now wait for a reply from the server
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (manager.recvfromAckTimeout(buf, &len, timeout, &from))
    {
      //      Serial.print("got reply from : 0x");
      //      Serial.print(from, HEX);
      //      Serial.print(": ");
      //      Serial.println((char*)buf);
      if (strcmp(buf, signal_code)) {
        Serial.println("Successfully sent!");
      }
      else Serial.println("Error!!");

    }
  }
  else
  {
    Serial.println("No reply, the train is nowhere near!");
  }
  delay(1000);

  //  radio.sleep();
  //  Watchdog.sleep(1000);     // USB will disconnect if we use this
}

// WIP needs practical testing
// Train receiver
// Receivers start with recvfromACk and address should be transmitter

#include <RHReliableDatagram.h>
#include <RH_RF95.h>
#include <SPI.h>
#include <LiquidCrystal.h>
#include <Adafruit_SleepyDog.h>
#include <ezBuzzer.h>

// These addresses need further study
// till then keep address same in post and train module
#define POST_ADDRESS 1 //signal post
#define TRAIN_ADDRESS 2 //train 

// for Feather32u4 RFM9x
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 7

int redLed = 5, y1Led = A2, y2Led = 404, greenLed = A3;
int pot = A0;
const int timeout = 2000; // timeout for retries

RH_RF95 driver(RFM95_CS, RFM95_INT); // select 32u4

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram manager(driver, POST_ADDRESS);

const int rs = 9, en = 6, d4 = 13, d5 = 12, d6 = 11, d7 = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);



const int BUZZER_PIN = A4;
ezBuzzer buzzer(BUZZER_PIN); // create ezBuzzer object that attach to a pin;
// notes in the melody:
int melody1[] = {
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5,
  NOTE_E5,
  NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
  NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5,
  NOTE_D5, NOTE_G5
};
int melody2[] = {
  NOTE_D5, NOTE_D5
};

// note durations: 4 = quarter note, 8 = eighth note, etc, also called tempo:
int noteDurations1[] = {
  8, 8, 4,
  8, 8, 4,
  8, 8, 8, 8,
  2,
  8, 8, 8, 8,
  8, 8, 8, 16, 16,
  8, 8, 8, 8,
  4, 4
};

int noteDurations2[] = {
  8, 8
};


// Function to display message on LCD
void displayMessage(String message, int line) {
  lcd.setCursor(0, line);
  String spaces = String("        "); //to fill the spaces
  lcd.print(message + spaces);
}


void Buzzer1() {
  buzzer.loop();
  if (buzzer.getState() == BUZZER_IDLE) {
    int length = sizeof(noteDurations1) / sizeof(int);
    buzzer.playMelody(melody1, noteDurations1, length); // playing
  }
}

void Buzzer2() {
  buzzer.loop();
  if (buzzer.getState() == BUZZER_IDLE) {
    int length = sizeof(noteDurations2) / sizeof(int);
    buzzer.playMelody(melody2, noteDurations2, length); // playing
  }
}
void decodeMessage(String sig) {  //RY1Y2G
  int red   = sig[0] - '0'; // convert string to int
  int y1    = sig[1] - '0';
  int y2    = sig[2] - '0';
  int green = sig[3] - '0';

  //  Serial.println(red);
  if (red) {
    digitalWrite(redLed, HIGH);
    Buzzer1();
  }
  else {
    digitalWrite(redLed, LOW);
  }
  if (y1) {
    digitalWrite(y1Led, HIGH);
    Buzzer2();
  } else {
    digitalWrite(y1Led, LOW);
  }
  if (y2) {
    digitalWrite(y2Led, HIGH);
    Buzzer2();
  }
  else {
    digitalWrite(y2Led, LOW);
  }
  if (green) {
    digitalWrite(greenLed, HIGH);
  } else {
    digitalWrite(greenLed, LOW);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) ; // Wait for serial port to be available
  if (!manager.init())
    Serial.println("init failed");
  //displayMessage("init failed", 0);
  driver.setFrequency(868.0);
  driver.setTxPower(20, false);

  pinMode(redLed, OUTPUT);
  pinMode(y1Led, OUTPUT);
  pinMode(y2Led, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(pot, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  lcd.begin(16, 2);
  lcd.clear();
}

uint8_t signal_code[RH_RF95_MAX_MESSAGE_LEN]; // RY1Y2G

void loop() {
  // put your main code here, to run repeatedly:
  int power = map(analogRead(pot), 0, 1023, 2, 20); // maps pot 2-20
  driver.setTxPower(power, false);

  //  displayMessage("RSSI" + String(driver.lastRssi()), 1);

  if (manager.available())
  {
    // Wait for a message addressed to us from the client
    uint8_t len = sizeof(signal_code);
    uint8_t from;
    if (manager.recvfromAck(signal_code, &len, &from))
    {
      //      Serial.print("got request from : 0x");
      //      Serial.print(from, HEX);
      //      Serial.print(": ");
      //      Serial.println((char*)signal_code);

      decodeMessage((char*)signal_code);
      displayMessage((char*)signal_code, 0);

      // Send a reply back to the originator client
      if (!manager.sendtoWait(signal_code, sizeof(signal_code), from))
        Serial.println("sendtoWait failed");
    }
  }
  else {
    Serial.println("No message received drive at full speed");

    delay(1000);

    //  radio.sleep();
    //  Watchdog.sleep(1000);     // USB will disconnect if we use this
  }

}

#include <ezBuzzer.h> // ezBuzzer library
#include <LiquidCrystal.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(8, 7);
int red = 12, yellow = 11, green = 13;
const byte address[6] = "00005";
const int BUZZER_PIN = 9; //buzzer to arduino pin 9
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
//const byte interruptPin = 2;

// Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(38, 36, 2, 3, 4, 5); /*RS = digi 38, E = digi 36, d4 = digi 3, d5 = digi 4, d6 = digi 5, d7 = digi 6*/

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
  //pinMode(BUZZER_PIN, OUTPUT); // Set buzzer - pin 9 as an output

  //pinMode(interruptPin, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // Clears the LCD screen
  lcd.clear();
}
int signalCode = 0;
void loop() {
  bool goodSignal = radio.testRPD();
  // put your main code here, to run repeatedly:
  if (radio.available()) {
    Serial.print(goodSignal ? "S\t" : "W\t" );
    lcd.setCursor(0, 1);

    lcd.print(goodSignal ? "Strong signal > 64dBm" : "Weak signal < 64dBm" );
    //radio.read(0, 0);+
    radio.read(&signalCode, sizeof(signalCode));
    //Serial.println("Radio available");
    if (signalCode == 2) {
      digitalWrite(yellow, LOW);
      digitalWrite(green, LOW);
      digitalWrite(red, HIGH);

      /*
        tone(BUZZER_PIN, 1000); // Send 1KHz sound signal...
        delay(500);        // ...for 1 sec
        noTone(BUZZER_PIN);     // Stop sound...
        delay(100);        // ...for 1sec
      */
      buzzer.loop();
      if (buzzer.getState() == BUZZER_IDLE) {
        int length = sizeof(noteDurations1) / sizeof(int);
        buzzer.playMelody(melody1, noteDurations1, length); // playing
      }
      lcd.setCursor(0, 0);
      lcd.print("Red            ");

    }
    else if (signalCode == 3) {
      digitalWrite(red, LOW);
      digitalWrite(green, LOW);
      digitalWrite(yellow, HIGH);
      /*
            tone(BUZZER_PIN, 100); // Send 1KHz sound signal...
            delay(1000);        // ...for 1 sec
            noTone(BUZZER_PIN);     // Stop sound...
            delay(1000);        // ...for 1sec
      */
      buzzer.loop();
      if (buzzer.getState() == BUZZER_IDLE) {
        int length = sizeof(noteDurations2) / sizeof(int);
        buzzer.playMelody(melody2, noteDurations2, length); // playing
      }
      //buzzer.beep(100);
      lcd.setCursor(0, 0);
      lcd.print("Yellow             ");
    }
    else if (signalCode == 4) {
      digitalWrite(yellow, LOW);
      digitalWrite(red, LOW);
      digitalWrite(green, HIGH);
      lcd.setCursor(0, 0);
      lcd.print("Green          ");
    } else {
      digitalWrite(red, LOW);
      digitalWrite(yellow, LOW);
      digitalWrite(green, LOW);
      lcd.setCursor(0, 0);

      lcd.print("No Signal        ");

    }
  } else {
    digitalWrite(red, LOW);
    digitalWrite(yellow, LOW);
    digitalWrite(green, LOW);
    lcd.setCursor(0, 0);
    lcd.print("Not available");
  }

  Serial.println(signalCode);

}
// Print a message to the LCD.lcd.setCursor(0, 0);



// set the cursor to column 0, line 1
// (note: line 1 is the second row, since counting begins with 0):

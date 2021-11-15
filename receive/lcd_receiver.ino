#include <LiquidCrystal.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(8, 7);
int red = 12, yellow = 11, green = 13;
const byte address[6] = "00005";

// Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(38, 36, 2, 3, 4, 5); /*RS = 4digi 6, E = digi 10, d4 = digi 2, d5 = digi 3, d6 = digi 4, d7 = digi 5*/

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

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // Clears the LCD screen
  lcd.clear();
}
int signalCode = 0;
void loop() {

  // put your main code here, to run repeatedly:
  if (radio.available()) {
    radio.read(&signalCode, sizeof(signalCode));
    //Serial.println("Radio available");
    if (signalCode == 2) {
      digitalWrite(yellow, LOW);
      digitalWrite(green, LOW);
      digitalWrite(red, HIGH);

      lcd.setCursor(0, 0);
      lcd.print("Red            ");

    }
    else if (signalCode == 3) {
      digitalWrite(red, LOW);
      digitalWrite(green, LOW);
      digitalWrite(yellow, HIGH);
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

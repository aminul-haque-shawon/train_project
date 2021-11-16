#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(8, 7);
int red = 2, yellow = 3, green = 4, maestro = 9;
const byte address[6] = "00005";
int button;
int second_button;
int signalCode = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  pinMode(red, INPUT);

  pinMode(yellow, INPUT);
  pinMode(green, INPUT);
  pinMode(maestro, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

  //char text[]="Hello Rimo";
 
  //radio.write(&text,sizeof(text));
  // red = 2,yellow=3,green=4//

  /* if(switch_debounce(red)==HIGH){
     signalCode=2;
     if (switch_debounce(green)==HIGH){
       OFF();
       signalCode = 4;
       }
     else if (switch_debounce(yellow)==HIGH){
       OFF();
       signalCode = 3;
       }

    }else if(switch_debounce(yellow)==HIGH){
     signalCode=3;
     if (switch_debounce(green)==HIGH){
       OFF();
       signalCode = 4;
       }
     else if (switch_debounce(red)==HIGH){
       OFF();
       signalCode = 2;
       }
    }else if(switch_debounce(green)==HIGH){
     signalCode=4;
     if (switch_debounce(yellow)==HIGH){
       OFF();
       signalCode = 3;
       }
     else if (switch_debounce(red)==HIGH){
       OFF();
       signalCode = 2;
       }
    }*/

  if (digitalRead(maestro) == HIGH ) {
    button = 0;
  }
  else if (digitalRead(red) == HIGH ) {
    button = 2; 
      }
  else if (digitalRead(yellow) == HIGH) {
    button = 3;
  }
  else if (digitalRead(green) == HIGH) {
    button = 4;
  }

  if (button == 0) {
    signalCode = 0;
  }
  else if (button == 2) {
    signalCode = 2;
  }
  else if (button == 3) {
    signalCode = 3;
  }
  else if (button == 4) {
    signalCode = 4;
  }


  radio.write(&signalCode, sizeof(signalCode));

  Serial.println(signalCode);



  //delay(1000);

}

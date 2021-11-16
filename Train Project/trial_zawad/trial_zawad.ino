


int switch_pin = 2;
int led_pin = 13;
boolean last_button = LOW;
boolean current_button = LOW;
boolean already_on = false;
void setup() {
  pinMode(switch_pin, INPUT);
  pinMode(led_pin, OUTPUT);
  Serial.begin(9600);
  }

/*boolean debounce(boolean last){
boolean current = digitalRead(switch_pin);
  if (last != current){
    delay(20);
    current = digitalRead(switch_pin);
    }
  return current;
  }*/

void loop(){
  if (digitalRead(switch_pin) == HIGH && already_on == false){
    //current_button = debounce(last_button);
    //if (last_button == LOW && current_button == HIGH){
    //already_on =! already_on;
    last_button = HIGH;
    digitalWrite(led_pin, HIGH);
    }
    else {
    digitalWrite(led_pin, LOW);
    //last_button = current_button;
    already_on = false;}
    
    digitalWrite(led_pin, already_on);
    Serial.println(already_on);
  }

const byte ledPin = 13;
const byte interruptPin = 2;
volatile byte state = HIGH;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);
}

void loop() {
  delay (1500);
  digitalWrite(ledPin, state);
  delay(1500);
  digitalWrite(ledPin, !state);
  Serial.println(interruptPin);
}

void blink() {
  int i = 0;
  while(1)
  {
  digitalWrite(ledPin, state);
  delay(100);
  digitalWrite(ledPin, !state);
  delay(100);}
  i++;
}

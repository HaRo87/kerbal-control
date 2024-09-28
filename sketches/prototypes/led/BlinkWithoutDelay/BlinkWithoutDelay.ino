const int ledPinRed = 2;  // the number of the red LED pin
const int ledPinYellow = 7;  // the number of the yellow LED pin
const int ledPinGreen = 8;  // the number of the green LED pin
const int RED = 0;
const int YELLOW = 1;
const int GREEN = 2;
int ledState = RED;  // ledState used to set the LED

const int buttonPinRed = 4;
int buttonStateRed = LOW;

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;  // will store last time LED was updated

// constants won't change:
const long interval = 500;  // interval at which to blink (milliseconds)

void setup() {
  // set the digital pin as output:
  pinMode(ledPinRed, OUTPUT);
  pinMode(ledPinYellow, OUTPUT);
  pinMode(ledPinGreen, OUTPUT);
  pinMode(buttonPinRed, INPUT);
}

void loop() {
  // here is where you'd put code that needs to be running all the time.

  // check to see if it's time to blink the LED; that is, if the difference
  // between the current time and last time you blinked the LED is bigger than
  // the interval at which you want to blink the LED.
  unsigned long currentMillis = millis();
  buttonStateRed = digitalRead(buttonPinRed);
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (buttonStateRed == HIGH) {
      if (ledState == RED) {
        digitalWrite(ledPinRed, HIGH);
        digitalWrite(ledPinYellow, LOW);
        digitalWrite(ledPinGreen, LOW);
        ledState = YELLOW;
      } else if (ledState == YELLOW) {
        digitalWrite(ledPinRed, LOW);
        digitalWrite(ledPinYellow, HIGH);
        digitalWrite(ledPinGreen, LOW);
        ledState = GREEN;
      } else {
        digitalWrite(ledPinRed, LOW);
        digitalWrite(ledPinYellow, LOW);
        digitalWrite(ledPinGreen, HIGH);
        ledState = RED;
      }
    } else {
      digitalWrite(ledPinRed, LOW);
      digitalWrite(ledPinYellow, LOW);
      digitalWrite(ledPinGreen, LOW);
    }
    

    // set the LED with the ledState of the variable:
    
  }
}

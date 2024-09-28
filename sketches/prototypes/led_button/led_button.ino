/* KerbalSimpitAltitudeTrigger
   A demonstration of subscribing to telemetry data from the game.
   Subscribes to the altitude channel, and turns the pin 13 LED
   on when the sea level altitude > 500m.

   Peter Hardy <peter@hardy.dropbear.id.au>
*/
#include "KerbalSimpit.h"

// Declare a KerbalSimpit object that will
// communicate using the "Serial" device.
KerbalSimpit mySimpit(Serial);
const int ledPinRed = 2;  // the number of the red LED pin
const int ledPinYellow = 7;  // the number of the yellow LED pin
const int ledPinGreen = 8;  // the number of the green LED pin
const int buttonPinRed = 4;
const int buttonPinYellow = 13;
const int buttonPinGreen = 12;
int buttonStateRed = LOW;
int buttonStateYellow = LOW;
int buttonStateGreen = LOW;
int lastButtonStateRed = LOW;
int lastButtonStateYellow = LOW;
int lastButtonStateGreen = LOW;

// the following variables are unsigned long's because the time, measured
// in miliseconds, will quickly become a bigger number than can be stored
// in an int.
unsigned long lastDebounceTimeRed = 0;  // the last time the output pin
                                     // was toggled
unsigned long lastDebounceTimeYellow = 0;  // the last time the output pin
                                     // was toggled
unsigned long lastDebounceTimeGreen = 0;  // the last time the output pin
                                     // was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase
                                     // if the output flickers
void setup() {
  // Open the serial connection.
  Serial.begin(115200);

  // Set up the build in LED, and turn it on.
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  pinMode(ledPinRed, OUTPUT);
  pinMode(ledPinYellow, OUTPUT);
  pinMode(ledPinGreen, OUTPUT);
  pinMode(buttonPinRed, INPUT_PULLUP);
  pinMode(buttonPinYellow, INPUT_PULLUP);
  pinMode(buttonPinGreen, INPUT_PULLUP);
  // This loop continually attempts to handshake with the plugin.
  // It will keep retrying until it gets a successful handshake.
  while (!mySimpit.init()) {
    delay(100);
  }
  // Turn off the built-in LED to indicate handshaking is complete.
  digitalWrite(LED_BUILTIN, LOW);
  // Display a message in KSP to indicate handshaking is complete.
  mySimpit.printToKSP("Connected", PRINT_TO_SCREEN);
  // Sets our callback function. The KerbalSimpit library will
  // call this function every time a packet is received.
  mySimpit.inboundHandler(messageHandler);
  // Send a message to the plugin registering for the Altitude channel.
  // The plugin will now regularly send Altitude messages while the
  // flight scene is active in-game.
  mySimpit.registerChannel(ALTITUDE_MESSAGE);
}

void loop() {
  // Check for new serial messages.
  mySimpit.update();
}

void messageHandler(byte messageType, byte msg[], byte msgSize) {
  // Read the state of the switch into a local variable.
  int readingRed = digitalRead(buttonPinRed);
  int readingYellow = digitalRead(buttonPinYellow);
  int readingGreen = digitalRead(buttonPinGreen);
  // If the switch changed, due to noise or pressing:
  if (readingRed != lastButtonStateRed) {
    // reset the debouncing timer
    lastDebounceTimeRed = millis();
  }

  if ((millis() - lastDebounceTimeRed) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (readingRed != buttonStateRed) {
      buttonStateRed = readingRed;

      // If the new button state is HIGH, that means the button
      // has just been pressed.
      if (buttonStateRed == HIGH) {
        // Send a message to the plugin activating the Stage
        // action group. The plugin will then activate the
        // next stage.
        mySimpit.activateAction(STAGE_ACTION);
      }
    }
  }
  if (readingYellow != lastButtonStateYellow) {
    // reset the debouncing timer
    lastDebounceTimeYellow = millis();
  }

  if ((millis() - lastDebounceTimeYellow) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (readingYellow != buttonStateYellow) {
      buttonStateYellow = readingYellow;

      // If the new button state is HIGH, that means the button
      // has just been pressed.
      if (buttonStateYellow == HIGH) {
        // Send a message to the plugin activating the Stage
        // action group. The plugin will then activate the
        // next stage.
        mySimpit.activateAction(LIGHT_ACTION);
      }
    }
  }
  if (readingGreen != lastButtonStateGreen) {
    // reset the debouncing timer
    lastDebounceTimeGreen = millis();
  }

  if ((millis() - lastDebounceTimeGreen) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (readingGreen != buttonStateGreen) {
      buttonStateGreen = readingGreen;

      // If the new button state is HIGH, that means the button
      // has just been pressed.
      if (buttonStateGreen == HIGH) {
        // Send a message to the plugin activating the Stage
        // action group. The plugin will then activate the
        // next stage.
        mySimpit.activateAction(SAS_ACTION);
      }
    }
  }
  lastButtonStateRed = readingRed;
  lastButtonStateYellow = readingYellow;
  lastButtonStateGreen = readingGreen;
  switch(messageType) {
  case ALTITUDE_MESSAGE:
    // Checking if the message is the size we expect is a very basic
    // way to confirm if the message was received properly.
    if (msgSize == sizeof(altitudeMessage)) {
      // Create a new Altitude struct
      altitudeMessage myAltitude;
      // Convert the message we received to an Altitude struct.
      myAltitude = parseMessage<altitudeMessage>(msg);
      // Turn the LED on if the vessel is higher than 500 metres
      // above sea level. Otherwise turn it off.
      if (myAltitude.sealevel < 5000) {
        digitalWrite(ledPinRed, HIGH);
        digitalWrite(ledPinYellow, LOW);
        digitalWrite(ledPinGreen, LOW);
      } else if ((myAltitude.sealevel >= 5000) && (myAltitude.sealevel < 70000)) {
        digitalWrite(ledPinRed, LOW);
        digitalWrite(ledPinYellow, HIGH);
        digitalWrite(ledPinGreen, LOW);
      } else if (myAltitude.sealevel >= 70000) {
        digitalWrite(ledPinRed, LOW);
        digitalWrite(ledPinYellow, LOW);
        digitalWrite(ledPinGreen, HIGH);
      }
    }
    break;
  }
}

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
#define LED_PIN_RED 2  // the number of the red LED pin
#define LED_PIN_YELLOW 7  // the number of the yellow LED pin
#define LED_PIN_GREEN 8  // the number of the green LED pin
#define BUTTON_PIN_RED 4
#define BUTTON_PIN_YELLOW 13
#define BUTTON_PIN_GREEN 12
#define PIN_TRANSLATION_X A0
#define PIN_TRANSLATION_Y A2

//Define values for the deadzones on the joystick
#define THC_X_DEADZONE_MIN (526 - 20)
#define THC_X_DEADZONE_MAX (526 + 20)
#define THC_Y_DEADZONE_MIN (535 - 40)
#define THC_Y_DEADZONE_MAX (535 + 40)
#define DEADZONE 3000
#define MAX_VOLT 1019

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
  pinMode(LED_PIN_RED, OUTPUT);
  pinMode(LED_PIN_YELLOW, OUTPUT);
  pinMode(LED_PIN_GREEN, OUTPUT);
  pinMode(BUTTON_PIN_RED, INPUT_PULLUP);
  pinMode(BUTTON_PIN_YELLOW, INPUT_PULLUP);
  pinMode(BUTTON_PIN_GREEN, INPUT_PULLUP);
  // pinMode(PIN_TRANSLATION_X, INPUT);
	// pinMode(PIN_TRANSLATION_Y, INPUT);
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
  // while(true)
	// {
  //   int sensorValue = 0; // up and down value
  //   int sensorValue1 = 0; // left and right value
  //   sensorValue = analogRead(PIN_TRANSLATION_X); // read pin A0
  //   sensorValue1 = analogRead(PIN_TRANSLATION_Y); // read pin A1
	// 	Serial.print("sensorx = "); // present in serial monitor up and down value
  //   Serial.println(sensorValue);
  //   Serial.print("sensory = "); // present in serial monitor left and right value
  //   Serial.println(sensorValue1);

	// 	delay(100);
	// }
  mySimpit.update();
  // Read the state of the switch into a local variable.
  int readingRed = digitalRead(BUTTON_PIN_RED);
  int readingYellow = digitalRead(BUTTON_PIN_YELLOW);
  int readingGreen = digitalRead(BUTTON_PIN_GREEN);
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
        mySimpit.toggleAction(LIGHT_ACTION);
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
        mySimpit.toggleAction(SAS_ACTION);
      }
    }
  }
  lastButtonStateRed = readingRed;
  lastButtonStateYellow = readingYellow;
  lastButtonStateGreen = readingGreen;
  mySimpit.update();
  int analogInputX = analogRead(PIN_TRANSLATION_X);
  // if (analogInputX > MAX_VOLT){analogInputX=MAX_VOLT;}
  int pitchX = 0;
  // pitchX = map(analogInputX, 0, MAX_VOLT, -32768, 32767);
  //Map the analog input to it's according value between the min/max and the deadzone. Leave it 0 when in the deadzone
  // if (analogInputX < THC_X_DEADZONE_MIN) {
  //   pitchX = map(analogInputX, THC_X_DEADZONE_MAX, 1023,   0, INT16_MAX);
  // } else if(analogInputX > THC_X_DEADZONE_MAX) {
  //   pitchX = map(analogInputX, 0, THC_X_DEADZONE_MIN,      INT16_MIN, 0);
  // }
  if (analogInputX < THC_X_DEADZONE_MIN or analogInputX > THC_X_DEADZONE_MAX) {
    pitchX = map(analogInputX, 0, 1023, INT16_MIN, INT16_MAX);
  }
  int analogInputY = analogRead(PIN_TRANSLATION_Y);
  int yawY = 0;
  //Map the analog input to it's according value between the min/max and the deadzone. Leave it 0 when in the deadzone
  // if (analogInputY < THC_Y_DEADZONE_MIN) {
  //   yawY = map(analogInputY, 0, THC_Y_DEADZONE_MIN,      INT16_MIN, 0);
  // } else if(analogInputY > THC_Y_DEADZONE_MAX) {
  //   yawY = map(analogInputY, THC_Y_DEADZONE_MAX, 1023,   0, INT16_MAX);
  // }
  if (analogInputY < THC_Y_DEADZONE_MIN or analogInputY > THC_Y_DEADZONE_MAX) {
    yawY = map(analogInputY, 0, 1023, INT16_MIN, INT16_MAX);
  }
  int translationZ = 0;

  rotationMessage rotation_msg;
  rotation_msg.setPitch(pitchX);
  rotation_msg.setYaw(yawY);
  rotation_msg.setRoll(0);
  rotation_msg.mask = 5;
  mySimpit.send(ROTATION_MESSAGE, rotation_msg);
  // translation_msg.setX(translationX);
  // translation_msg.setY(translationY);
  // translation_msg.setZ(translationZ);
  // mySimpit.send(TRANSLATION_MESSAGE, translation_msg);
  // Check for new serial messages.
  mySimpit.update();
  // if (translationX > 0) {
  //   mySimpit.printToKSP("X:", PRINT_TO_SCREEN);
  //   mySimpit.printToKSP(String(translationX), PRINT_TO_SCREEN);
  // }
  // if (translationY > 0) {
  //   mySimpit.printToKSP("Y:", PRINT_TO_SCREEN);
  //   mySimpit.printToKSP(String(translationY), PRINT_TO_SCREEN);
  // }
}

void messageHandler(byte messageType, byte msg[], byte msgSize) {
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
        digitalWrite(LED_PIN_RED, HIGH);
        digitalWrite(LED_PIN_YELLOW, LOW);
        digitalWrite(LED_PIN_GREEN, LOW);
      } else if ((myAltitude.sealevel >= 5000) && (myAltitude.sealevel < 70000)) {
        digitalWrite(LED_PIN_RED, LOW);
        digitalWrite(LED_PIN_YELLOW, HIGH);
        digitalWrite(LED_PIN_GREEN, LOW);
      } else if (myAltitude.sealevel >= 70000) {
        digitalWrite(LED_PIN_RED, LOW);
        digitalWrite(LED_PIN_YELLOW, LOW);
        digitalWrite(LED_PIN_GREEN, HIGH);
      }
    }
    break;
  }
}

int deadZone(int input){
  if (input > 3000 or input < -3000){
    return input;
  } else {
    return 0;
  }
}

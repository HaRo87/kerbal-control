#include <LiquidCrystal_I2C.h>
#include "KerbalSimpit.h"

// Set the pin numbers:
const int THROTTLE_PIN = A3; // the pin used for controlling throttle
LiquidCrystal_I2C lcd(0x27, 16, 2);
// Declare a KerbalSimpit object that will
// communicate using the "Serial" device.
KerbalSimpit mySimpit(Serial);

void setup() {
  lcd.init(); // initialize the lcd
  lcd.backlight();
  // Open the serial connection.
  Serial.begin(115200);

  // Set initial pin states  
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Turn on the built-in to indicate the start of the handshake process
  digitalWrite(LED_BUILTIN, HIGH); 

  // This loop continually attempts to handshake with the plugin.
  // It will keep retrying until it gets a successful handshake.
  while (!mySimpit.init()) {
    delay(100);
  }
  // Turn off the built-in LED to indicate handshaking is complete.
  digitalWrite(LED_BUILTIN, LOW);
  // Display a message in KSP to indicate handshaking is complete.
  mySimpit.printToKSP("Connected CTRL 2", PRINT_TO_SCREEN);
  lcd.clear();                 // clear display
  lcd.setCursor(0, 0);         // move cursor to   (0, 0)
  lcd.print("CONNECTED");        // print message at (0, 0)
  mySimpit.inboundHandler(messageHandler);
  // Send a message to the plugin registering for the channels.
  mySimpit.registerChannel(VELOCITY_MESSAGE);
}

void loop() {
  // Send at each loop a message to control the throttle and the pitch/roll axis.
  throttleMessage throttle_msg;
  // Read the value of the potentiometer
  int reading = analogRead(THROTTLE_PIN);
  // Convert it in KerbalSimpit Range
  throttle_msg.throttle = map(reading, 0, 1023, 0, INT16_MAX);
  // Send the message
  mySimpit.send(THROTTLE_MESSAGE, throttle_msg);
}

void messageHandler(byte messageType, byte msg[], byte msgSize) {
  switch(messageType) {
  case VELOCITY_MESSAGE:
    // Checking if the message is the size we expect is a very basic
    // way to confirm if the message was received properly.
    if (msgSize == sizeof(velocityMessage)) {
      // Create a new Altitude struct
      velocityMessage myVelocity;
      // Convert the message we received to an Altitude struct.
      myVelocity = parseMessage<velocityMessage>(msg);
      // Turn the LED on if the vessel is higher than 500 metres
      // above sea level. Otherwise turn it off.
      lcd.clear();                 // clear display
      lcd.setCursor(0, 0);         // move cursor to   (0, 0)
      lcd.print("Velocity (ORB)");        // print message at (0, 0)
      lcd.setCursor(2, 1);         // move cursor to   (2, 1)
      lcd.print(String(myVelocity.orbital)); // print message at (2, 1) 
      delay(200);
    }
    break;
  }
}

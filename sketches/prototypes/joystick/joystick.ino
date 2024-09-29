const int analogInPinY = A0; // input up down

const int analogInPinX = A2; // input left right

int sensorValueY = 0; // up and down value

int sensorValueX = 0; // left and right value

void setup() {

Serial.begin(115200); // enable serial monitor

}

void loop() {

sensorValueY = analogRead(analogInPinY); // read pin A0

sensorValueX = analogRead(analogInPinX); // read pin A1

// print the results to the Serial Monitor:

Serial.print("sensorY = "); // present in serial monitor up and down value

Serial.println(sensorValueY);

Serial.print("sensorX = "); // present in serial monitor left and right value

Serial.println(sensorValueX);

delay(500); } // delay in milliseconds 

const int analogInPinY = A0; // input up down

const int analogInPinX = A2; // input left right

int sensorValueY = 0; // up and down value

int sensorValueX = 0; // left and right value

#define DEADZONE_CENTER_X 526
#define DEADZONE_CENTER_Y 535
#define DEADZONE_CENTER_Z 0
#define DEADZONE 40

void setup() {

Serial.begin(115200); // enable serial monitor

}

void loop() {

sensorValueY = analogRead(analogInPinY); // read pin A0
int translationY = 0;
//Map the analog input to it's according value between the min/max and the deadzone. Leave it 0 when in the deadzone
if((DEADZONE_CENTER_Y - DEADZONE) < sensorValueY && sensorValueY < (DEADZONE_CENTER_Y + DEADZONE)){
  translationY = 0;
} else {
  translationY = constrain(map(sensorValueY,0,1023,INT16_MIN,INT16_MAX),INT16_MIN,INT16_MAX); //x
}
sensorValueX = analogRead(analogInPinX); // read pin A1
int translationX = 0;
//Map the analog input to it's according value between the min/max and the deadzone. Leave it 0 when in the deadzone
if((DEADZONE_CENTER_X - DEADZONE) < sensorValueX && sensorValueX < (DEADZONE_CENTER_X + DEADZONE)){
  translationX = 0;
} else {
  translationX = constrain(map(sensorValueX,0,1023,INT16_MIN,INT16_MAX),INT16_MIN,INT16_MAX); //x
}
// print the results to the Serial Monitor:

Serial.print("sensorY = "); // present in serial monitor up and down value

Serial.println(translationY);
Serial.println(sensorValueY);
Serial.print("sensorX = "); // present in serial monitor left and right value

Serial.println(translationX);
Serial.println(sensorValueX);
delay(500); } // delay in milliseconds 

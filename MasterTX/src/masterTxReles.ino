#include <Wire.h>

int nodoReles=2;
int nodoTv=3;
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
char command[10];

unsigned long previosMillis = 0;
unsigned long interval = 1000;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  Wire.begin();
  // reserve 200 bytes for the inputString:
  inputString.reserve(10);
  //Serial.println("reset");
}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    //Serial.println(inputString);
    inputString.toCharArray(command, 10);
    //Serial.println(command);
    //Serial.println("enviar");
    Wire.beginTransmission(nodoReles); // transmit to relays
    Wire.write(command);
    delay(10);
    Wire.endTransmission();    // stop transmitting
    //Wire.beginTransmission(nodoTv); // transmit to relays
    //Wire.write(command);
    //Wire.endTransmission();    // stop transmitting
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  /*
  unsigned long currentMillis = millis();
   if(currentMillis - previosMillis > interval){
   previosMillis = currentMillis;

   Wire.requestFrom(3, 9);    // request 9 bytes from slave device #3
   while(Wire.available())    // slave may send less than requested
   {
   char c = Wire.read(); // receive a byte as character
   inputString += c;
   if (c == '\n') {
   stringComplete = true;
   }
   }
   }*/
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    if (inChar == 'A'){
      //Serial.print("status");
      //MASTER Request
      Wire.requestFrom(2, 8);    // request 6 bytes from slave device #2
      while(Wire.available())    // slave may send less than requested
      {
        char c = Wire.read(); // receive a byte as character
        Serial.print(c);         // print the character
      }
    }
    else{
      inputString += inChar;
      if (inChar == '\n') {
        stringComplete = true;
      }
    }
  }
}

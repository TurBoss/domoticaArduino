#include <Wire.h>

int latchPin = 8; //Pin connected to ST_CP of 74HC595
int clockPin = 12; //Pin connected to SH_CP of 74HC595
int dataPin = 11; ////Pin connected to DS of 74HC595

int numberToDisplay=0;
String inputString = "00000000";         // a string to hold incoming data
boolean stringComplete = true;  // whether the string is complete
char command[10];

void setup()
{
  Serial.begin(9600);           // start serial for output

  Wire.begin(2);                // join i2c bus with address #2
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent); // register event

  pinMode(latchPin, OUTPUT);    //control del 595
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  inputString.reserve(9);
}

int numero;
int bitRele, exponente=1;

void loop()
{
  if (stringComplete) {
    //Serial.println(inputString);
    inputString.toCharArray(command, 9);
    Serial.println(command);
    for(int i=0; i < inputString.length(); i++){
      numero=command[i]-48;
      Serial.print(numero);
      Serial.print(" ");
      bitRele=numero;
      //exponente=pow(2,7-i);
      for(int j=0; j<7-i; j++)
        exponente=2*exponente;
      numberToDisplay+=bitRele*exponente; //bin to dec
      Serial.print(exponente);
      exponente=1;
      Serial.print(" ");
      Serial.println(numberToDisplay);
    }

    Serial.println();
    //Serial.print("numberToDisplay= ");
    Serial.println(numberToDisplay);
    Serial.println();

    digitalWrite(latchPin, LOW);    // take the latchPin low so the LEDs don't change while you're sending in bits:
    shiftOut(dataPin, clockPin, MSBFIRST, numberToDisplay);    // shift out the bits:
    digitalWrite(latchPin, HIGH);    //take the latch pin high so the LEDs will light up:
    inputString = "";
    stringComplete = false;
    numberToDisplay=0;
  }
}

void receiveEvent(int howMany)
{
  while(Wire.available())
  {
    char inChar = (char)Wire.read();
    if (inChar == '\n') {
      stringComplete = true;
    }
    else
      inputString += inChar;
  }
}

String estado;

void requestEvent()
{
  command[9] = '\0';
  Wire.write(command);
}

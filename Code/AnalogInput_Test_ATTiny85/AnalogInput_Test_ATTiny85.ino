//This code is used to test the analog input of an ATTiny85

//Circuit:
//PWM pin -> 270 Ohm resistor -> LED -> Ground
//Potentiometer
//5V->potentiometer
//Wiper->Pin 2 of ATTiny85
//Gnd->potentiometer

//Note: A2 = physical pin 3, A3 = physical pin 2, A1 = phsical pin 7
byte anInPin = A2; //analog input pin used to read value of potentiometer
int minScaleVal = 10;  //Input should be between 0 - 1023
int maxScaleVal = 1000;
int outValMin = 1;  //output after scaling
int outValMax = 255;

byte led1Pin = 0;
byte led2Pin = 1;

uint8_t blinkVal;

void setup() 
{
  pinMode(anInPin, INPUT);
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT); 
}

uint8_t getHundredsDigit(int inVal)
{
  uint8_t retVal=inVal/100;
  return retVal;
}

uint8_t getTensDigit(int inVal)
{
  uint8_t retVal=inVal%100;
  retVal=retVal/10;
  return retVal;
}

//A quick flash of both LEDs indicates a value of 0.
void blinkDigitVal(uint8_t val)
{
  if(val==0)
  {
    digitalWrite(led1Pin, HIGH);    // light up the LED
    digitalWrite(led2Pin, HIGH);    // light up the LED
    delay(100);
    digitalWrite(led1Pin, LOW);    // turn off the LED
    digitalWrite(led2Pin, LOW);    // turn off the LED
    delay(400);
  }
  else
  {
    for(uint8_t i=0;i<val;i++)
    {
      digitalWrite(led1Pin, HIGH);    // light up the LED
      delay(400);
      digitalWrite(led1Pin, LOW);    // turn off the LED
      delay(400);
    }
  }
}

void loop() 
{  
  long anValIn = analogRead(anInPin);  
  if(anValIn<minScaleVal)
    anValIn=minScaleVal;
  if(anValIn>maxScaleVal)
    anValIn = maxScaleVal;
    
  //Scale the value
  int delayValScaled = outValMin + ((anValIn-minScaleVal)*(outValMax-outValMin))/(float)(maxScaleVal-minScaleVal);
  
  //Output the value using the LEDs to indicate
  int outVal = anValIn;
  //Begin blink
  digitalWrite(led2Pin,HIGH);
  delay(400);
  digitalWrite(led2Pin,LOW);
  delay(400);
  //Hundreds digit
  blinkVal = getHundredsDigit(outVal);
  blinkDigitVal(blinkVal);
  delay(1000);
  //Tens digit
  blinkVal = getTensDigit(outVal);
  blinkDigitVal(blinkVal);
  delay(1000);
}

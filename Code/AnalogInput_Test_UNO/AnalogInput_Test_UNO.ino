//This code is used to test the analog input of an UNO
//Once I get it working correctly I will transfer to an ATTiny85.
//A blinking LED will indicate the analog input value.

//Circuit:
//PWM pin -> 270 Ohm resistor -> LED -> Ground
//Potentiometer
//5V->potentiometer
//Wiper->Pin 2 of ATTiny85
//Gnd->potentiometer

byte anInPin = 0; //analog input pin used to read value of potentiometer
int minScaleVal = 10;  //Input should be between 0 - 1023
int maxScaleVal = 1000;
int outValMin = 1;  //output after scaling
int outValMax = 255;

byte led1Pin = 6;
byte led2Pin = 7;

uint8_t blinkVal;

void setup() 
{
  Serial.begin(9600);
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
  int outVal = delayValScaled;
  Serial.print("anValIn=");
  Serial.println(anValIn);
  Serial.print("outVal=");
  Serial.println(outVal);
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

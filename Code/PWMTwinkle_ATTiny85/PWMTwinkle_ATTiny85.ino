//This code is used to test creating the star twinkle effect
//by varying the LED intensity.
//It is intended to run on an ATTiny85
//It will create a twinkling effect for 3 LEDs.
//PWM outputs are on physical pin 3,2, and 7
//The twinkling effect can be modified by two analog inputs connected to potentiometers.

//Circuit:
//PWM pin -> 270 Ohm resistor -> LED -> Ground
//Potentiometer
//5V->potentiometer
//Wipter->Analog 0
//Gnd->potentiometer

#define NUM_LED 3 //Set the total number of LEDs

byte pwmPinArray[NUM_LED];
byte maxIntenArray[NUM_LED];
byte minIntenArray[NUM_LED];
int fadeStepArray[NUM_LED];
int currIntenArray[NUM_LED];
int currDelayValArray[NUM_LED];
int delayValArray[NUM_LED];
byte minIntenThresh = 40;   //Never let the random value for maxInten be below this value.
//Input 1: controls the speed of the fade in/out
//A lower value will be more blinky, a higher value will be more of a fading flame
//Note: A2 = physical pin 3, A3 = physical pin 2, A1 = phsical pin 7
byte anIn1Pin = A3; //analog input pin used to read value of potentiometer
int minScaleVal = 10;
int maxScaleVal = 1000;
int delayValMin = 1;
int delayValMax = 200;
int delayValScaled;
long delayValIn;
//Input 2: controls the delay between each fade in/out
//A higher value will be used for something like fireflies effect. Since
//they will not always be lit up.
//The sequence is: fade in->fade out->wait x milliseconds->fade in->fade out->wait x milliseconds->repeat
//the value of x is set by this input.
//Note: A2 = physical pin 3, A3 = physical pin 2, A1 = phsical pin 7
byte anIn2Pin = A1; //analog input pin used to read value of potentiometer
int minScaleVal2 = 10;
int maxScaleVal2 = 1000;
int delayValMin2 = 0;
int delayValMax2 = 1000;
int delayValScaled2;
long delayValIn2;


void setup() 
{
  //Initialize the random sequence. This is using the Reset pin, which is unconnected
  randomSeed(analogRead(A0));
  
  pinMode(anIn1Pin, INPUT);
  pinMode(anIn2Pin, INPUT);  
  //LED pins. NOTE: These are specified by the IO pin numbers.
  //IO Number    ->   Physical pin number
  //    0        ->      5
  //    1        ->      6
  //    4        ->      3
  pwmPinArray[0]=0;
  pwmPinArray[1]=1;
  pwmPinArray[2]=4;
  //Initializations that can be repeated for each LED
  for(int i=0;i<NUM_LED;i++)
  {
    pinMode(pwmPinArray[i], OUTPUT);
    //Initialize intensity
    maxIntenArray[i] = random(minIntenThresh,255);
    minIntenArray[i] = random(0,maxIntenArray[i]);
    currIntenArray[i]=minIntenArray[i];
    currDelayValArray[i]=0;
    delayValArray[i]=0;
  }
  //Fade step, this might be the same value for all LEDs
  fadeStepArray[0] = 10;
  fadeStepArray[1] = 10;
  fadeStepArray[2] = 10;
}

void loop() 
{  
  //Get the value used to set the delay between intensity updates.
  delayValIn = analogRead(anIn1Pin);  //Note: I am using long type to force the scaling calculation to use long.
  if(delayValIn<minScaleVal)
    delayValIn=minScaleVal;
  if(delayValIn>maxScaleVal)
    delayValIn = maxScaleVal;    
  //Scale the value
  delayValScaled = delayValMin + ((delayValIn-minScaleVal)*(delayValMax-delayValMin))/(float)(maxScaleVal-minScaleVal);
  
  //Get the value used to set the delay between fades.
  delayValIn2 = analogRead(anIn2Pin);  //Note: I am using long type to force the scaling calculation to use long.
  if(delayValIn2<minScaleVal2)
    delayValIn2=minScaleVal2;
  if(delayValIn2>maxScaleVal2)
    delayValIn2 = maxScaleVal2;    
  //Scale the value
  delayValScaled2 = delayValMin2 + ((delayValIn2-minScaleVal2)*(delayValMax2-delayValMin2))/(float)(maxScaleVal2-minScaleVal2);
  //Serial.print("delayValScaled2 = ");
  //Serial.println(delayValScaled2);
  
  for(int i=0;i<NUM_LED;i++)
  {
    //This is the gap between fade in/outs.
    //During this time the LED will be off.
    if(currDelayValArray[i]<delayValArray[i])
    {
      currDelayValArray[i]=currDelayValArray[i]+delayValScaled;
      analogWrite(pwmPinArray[i], 0);  //Set to 0
      continue;
    }
    //Begin the fade in/out
    //Set value
    analogWrite(pwmPinArray[i], currIntenArray[i]);
    //Fade (in or out depending on the value of fadeStep
    currIntenArray[i] = currIntenArray[i] + fadeStepArray[i];
    //Check if we reached the max or min intensity value
    if(currIntenArray[i]<minIntenArray[i])
    {     
      //We have reached the end of a fade in/out.
      fadeStepArray[i] = fadeStepArray[i]*-1;  //reverse the fade
      //Change the intensity range
      maxIntenArray[i] =random(minIntenThresh,255);
      minIntenArray[i] = random(0,maxIntenArray[i]);
      currIntenArray[i]=minIntenArray[i];
      //Reset the delay value used between fade in/outs
      //Allow the delay time to vary by +-10%
      //This should add some more variance into the pattern.
      int varAmt = (int)(delayValScaled2*0.1);
      delayValArray[i] = random(delayValScaled2-varAmt,delayValScaled2+varAmt);
      currDelayValArray[i] = 0;
    }
    else if(currIntenArray[i]>maxIntenArray[i])
    {
      //We have reached the end of a fade in. Now begin the fade out.
      //Serial.println("max reached");
      fadeStepArray[i] = fadeStepArray[i]*-1;  //reverse the fade    
      currIntenArray[i]=maxIntenArray[i];
    }
  }
    
  delay(delayValScaled); 
  
  /*
  delayValIn = analogRead(anIn1Pin);  //Note: I am using long type to force the scaling calculation to use long.
  if(delayValIn<minScaleVal)
    delayValIn=minScaleVal;
  if(delayValIn>maxScaleVal)
    delayValIn = maxScaleVal;
    
  //Scale the value
  int delayValScaled = delayValMin + ((delayValIn-minScaleVal)*(delayValMax-delayValMin))/(float)(maxScaleVal-minScaleVal);
      
  //fade in    
  for(int i=minInten1;i<maxInten1;i+=fadeStep1)
  {
    analogWrite(pwm1Pin, i);
    delay(delayValScaled);
  }
  
  //fade out  
  for(int i=maxInten1;i<minInten1;i-=fadeStep1)
  {
    analogWrite(pwm1Pin, i);
    delay(delayValScaled);
  }
  
  maxInten1 =random(0,255);
  minInten1 = random(0,maxInten1);
  */
}

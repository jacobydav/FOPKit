//This code is intended to run on a ATTiny85.
//It will blink an LED
void setup() 
{
  pinMode (1, OUTPUT);  // pin 6  // OC0B

}

void loop() 
{
  digitalWrite(1,255);
  delay(1000);
  digitalWrite(1,0);
  delay(500);

}

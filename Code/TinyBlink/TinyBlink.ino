#include "avr/sleep.h"

const int statusPin = 1;

void setup()
{
	pinMode(statusPin, OUTPUT);
	attachInterrupt(0, doSomething, FALLING);
}

void loop()
{
	delay(2000);
	digitalWrite(statusPin, LOW);
}

void doSomething(){
	digitalWrite(statusPin, HIGH);
}

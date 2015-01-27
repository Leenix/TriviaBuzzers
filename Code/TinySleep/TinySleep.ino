#include "avr/sleep.h"

#define STATUS_PIN 1
#define INTERRUPT_NUM 0
const int statusPin = 1;

void setup()
{
	pinMode(statusPin, OUTPUT);
	
}

void sleepNow()
{
	// Lowest level sleep - Highest power savings
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	attachInterrupt(INTERRUPT_NUM, buttonInterrupt, FALLING);
	sleep_mode();
}

void disableSleep(){
	detachInterrupt(INTERRUPT_NUM);
	sleep_disable();
}

void loop()
{
	sleepNow();

	disableSleep();
}

void buttonInterrupt(){
	
}

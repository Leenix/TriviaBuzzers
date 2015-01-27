#include <RF24_config.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <SPI.h>
#include "avr/sleep.h";

RF24 radio(8,7);

uint64_t coordinatorPipe = 0x0F0F0F0F11LL;
uint8_t payload[] = {'r'};
	
void setup()
{
	pinMode(3, OUTPUT);
	//set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	//sleep_enable();
	
	// Set up interrupts
	//TODO - May have to do this manually
	//attachInterrupt(0, INT0_ISR, FALLING);

	radio.begin();
	radio.openWritingPipe(coordinatorPipe);
}

void loop()
{
	digitalWrite(3, HIGH);
	radio.write(&payload, sizeof(payload));
	
	delay(500);
	digitalWrite(3, LOW);
	delay(500);

}

void INT0_ISR(){
	radio.write(&payload, sizeof(payload));
}
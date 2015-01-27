#include <RF24_config.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <SPI.h>
#include "avr/sleep.h"

const char BUZZER_ID = 'r';

#define STATUS_PIN 1	// Statis LED pin
#define DEFAULT_FLASH_PERIOD 50	// Time between LED ON/OFF transitions

#define BUZZER_LOCKOUT 500	// Minimum time between buzzing (in ms)

#define INTERRUPT_NUM 0	// Interrupt ID for the external button interrupt (INT0)
#define BUZZER_ID_ADDRESS 0	// EEPROM address that holds the buzzer ID character
#define PIPE_ID_ADDRESS 1	// EEPROM address that holds the pipe ID character

#define CE_PIN 8	// RF24 chip enable pin
#define CSN_PIN 7	// RF24 chip select not pin
RF24 radio(CE_PIN,CSN_PIN);

/**
* Base address for all network pipes
* Add the specific pipe ID to the base address to get the address for the specific buzzer
* Some buzzers will share pipes, hence why the pipe ID is not always equal to buzzer ID
*/
const uint64_t PIPE_ADDRESS = 0xE8E8F0F00001LL;
const int BUZZER_CHANNEL = 48;

//////////////////////////////////////////////////////////////////////////

/** 
* Initialization
*/
void setup()
{
	// Set up LED
	pinMode(STATUS_PIN, OUTPUT);
	flashStatusLED(1);
	delay(300);
		
	initialiseRadio();

	
	// Wrap up initialisation
	flashStatusLED(1);
}


/**
* Main Loop
*/
void loop()
{
	// Put device to sleep
	sleepNow();
	
	// Sleep breakpoint
	disableSleep();
	transmitID();
	
	delay(50);
	flashStatusLED(3);
	delay(BUZZER_LOCKOUT);
}

//////////////////////////////////////////////////////////////////////////


/**
* Start the RF24 radio
*/
void initialiseRadio()
{
	radio.begin();
	radio.setPALevel(RF24_PA_LOW);
	radio.setChannel(BUZZER_CHANNEL);
	radio.openWritingPipe(PIPE_ADDRESS);
	radio.powerDown();
}


/**
* Put the chip to sleep
* Only a reset, or a button interrupt can wake up the chip
*/
void sleepNow()
{
	// Lowest level sleep - Highest power savings
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	
	sleep_enable();
	
	// Enable periodic interrupts
	attachInterrupt(INTERRUPT_NUM, buttonInterrupt, FALLING);
	sleep_mode();
}


/**
* Disable chip sleep and button interrupts
*/
void disableSleep(){
	detachInterrupt(INTERRUPT_NUM);
	sleep_disable();
}


/**
* Transmit the buzzer's id to the base station 
* The RF24 module is used as the communications link
*/
void transmitID(){
	radio.powerUp();
	radio.write(&BUZZER_ID, sizeof(BUZZER_ID));
	radio.powerDown();
}


/**
* Button interrupt service routine
* Only called to wake up the device (for now)
*/
void buttonInterrupt(){
	
}


/**
* Flash the status LED for the specified number of times
* The flash period is set by DEFAULT_FLASH_PERIOD
*/
void flashStatusLED(int times){
	int delayPeriod = DEFAULT_FLASH_PERIOD;
	
	for (int i = 0; i < times; i++){
		digitalWrite(STATUS_PIN, HIGH);
		delay(delayPeriod);
		digitalWrite(STATUS_PIN, LOW);
		delay(delayPeriod);
	}
}


/**
* Error loop for when the system encounters a critical error
* Flash the status LED forever
*/
void errorLoop(){
	while(1){
		flashStatusLED(1);
	}
}

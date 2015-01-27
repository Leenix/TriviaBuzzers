#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN   9
#define CSN_PIN 10

#define PIEZO 2
#define RED1_PIN 3
#define GRN1_PIN 5
#define BLU1_PIN 6

#define RED2_PIN 9
#define GRN2_PIN 10
#define BLU2_PIN 11

#define LED1_PIN 12
#define LED2_PIN 13

const uint64_t pipe = 0xE8E8F0F000LL;
const char pipeIDs[] = {'y', 'g'};

RF24 radio(CE_PIN, CSN_PIN);

char payload[1];

void setup()
{
	
}


void loop()
{
		
}

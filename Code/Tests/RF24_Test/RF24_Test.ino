#include <SPI.h>
#include <RF24_config.h>
#include <RF24.h>
#include <nRF24L01.h>

RF24 radio(9, 10);

uint64_t transmitPipe = 0x0F0F0F0F01LL;
uint64_t receivePipe = 0x0F0F0F0F02LL;

char payload[] = {"butts"};
	
void setup()
{
	SPI.begin();
	radio.begin();

	radio.openReadingPipe(1,receivePipe);
	radio.openWritingPipe(transmitPipe);
}


void loop()
{
	radio.startListening();
	
	while(!radio.available()){
		
	}


}

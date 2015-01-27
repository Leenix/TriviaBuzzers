#include <SoftwareSerial.h>
#include <RgbStrip.h>
#include <RGB.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


#define BUZZER_PIN 2


// LED Strips
#define RED1_PIN 5
#define GRN1_PIN 3
#define BLU1_PIN 6
RgbStrip leftStrip(RED1_PIN, GRN1_PIN, BLU1_PIN);
bool leftLock = false;

#define RED2_PIN 10
#define GRN2_PIN 9
#define BLU2_PIN 11
RgbStrip rightStrip(RED2_PIN, GRN2_PIN, BLU2_PIN);
bool rightLock = false;

#define BLUETOOTH_TX A3
#define BLUETOOTH_RX A2
SoftwareSerial bluetooth(BLUETOOTH_RX, BLUETOOTH_TX);

// Indicator LEDs
#define LED1_PIN 12
#define LED2_PIN 13

//////////////////////////////////////////////////////////////////////////
// RF24 Configuration
#define CE_PIN  7
#define CSN_PIN 8
RF24 radio(CE_PIN, CSN_PIN);
const uint64_t BUZZER_PIPE = 0xE8E8F0F00001LL;
const int BUZZER_CHANNEL = 48;
char payload[1];
bool buzzersEnabled = false;

//////////////////////////////////////////////////////////////////////////
enum CONTROL_MODES{
	REMOTE_BOTH = '0',
	REMOTE_LEFT = '1',
	REMOTE_RIGHT = '2',
	TRIVIA = '4',
	READY_REGULATED = '5',
	MULTI_READY = '7',
};
char operatingMode;

enum ACTION_COMMANDS{
	// Strip-specific
	LIGHTS_OFF = 'Z',
	INCREASE_BRIGHTNESS = '+',
	DECREASE_BRIGHTNESS = '-',
	LOW_BRIGHTNESS = 'L',
	FULL_BRIGHTNESS = 'F',
	
	ENABLE_STROBE = 'S',
	DISABLE_STROBE = 'D',
	INCREASE_STROBE_PERIOD = ']',
	DECREASE_STROBE_PERIOD = '[',
	
	ENABLE_TRANSITIONS = 'T',
	DISABLE_TRANSITIONS = 'Y',
	INCREASE_TRANSITION_PERIOD = '.',
	DECREASE_TRANSITION_PERIOD = ',',
	
	RESET_STRIP = 'R',
	
	// Non-specific
	ENABLE_BUZZERS = 'B',
	DISABLE_BUZZERS = 'N',
	
	PRINT_STATUS = 'Q',
	PRINT_COMMAND_LIST = 'H',
	
	MODE_RESET = 'X'
};

//////////////////////////////////////////////////////////////////////////

/**
* Initialization - Run once
*/
void setup()
{
	Serial.begin(57600);
	bluetooth.begin(57600);
	
	initialiseRadio();
	initialiseLights();
}


/**
* Main Loop
*/
void loop()
{
	leftStrip.update();
	rightStrip.update();
	
	if(Serial.available()){
		char c = Serial.read();
		handleSerialChar(c);
	}
	
	if (bluetooth.available()){
		char c = bluetooth.read();
		handleSerialChar(c);
	}
	
	if (buzzersEnabled && radio.available())
	{
		//handleRadioChar();
	}
}

//////////////////////////////////////////////////////////////////////////

/**
* Start the RF24 radio and open listening pipes for buzzer communication
*/
void initialiseRadio(){
	radio.begin();
	
	radio.setChannel(BUZZER_CHANNEL);
	radio.openReadingPipe(1, BUZZER_PIPE);
	radio.startListening();
}


/**
* Quickly test the lights
*/
void initialiseLights(){
	setMode(REMOTE_BOTH);
	
	leftStrip.setColour(RED);
	rightStrip.setColour(BLUE);
}


/**
* Process the incoming Serial command
* Numbers are mode commands
* Lower case letters are colour commands
* Uppercase and symbols are action commands
*/
void handleSerialChar(char receivedChar){
	// Numbers are mapped to mode changes
	if (isDigit(receivedChar)){
		setMode(receivedChar);
	}
	
	// Upper case characters are mapped to commands
	else if (isLowerCase(receivedChar)){
		processColourCommand(receivedChar);
	}
	
	// Lower case characters are mapped to colours
	else{
		processActionCommand(receivedChar);
	}
}


/**
* Process commands coming from the RF24
* The RF24 is used to communicate with buzzers. Only colour commands should be received.
*/
void handleRadioChar(){
	radio.read(payload, sizeof(payload));
	
	char c = payload[0];
	processColourCommand(c);
}


/**
* Set the operating mode of the buzzer base controller
*/
void setMode(char c){
	operatingMode = c;
	
	switch (c){
		case REMOTE_LEFT:
		leftLock = false;
		rightLock = true;
		break;
		
		case REMOTE_RIGHT:
		rightLock = false;
		leftLock = true;
		break;
		
		default:
		rightLock = false;
		leftLock = false;
	}
	
}


/**
* Perform a command based on the input character
*/
void processActionCommand(char c){
	switch(c){
		case ENABLE_BUZZERS:
		enableBuzzers();
		break;
		
		case DISABLE_BUZZERS:
		disableBuzzers();
		break;
		
		case PRINT_STATUS:
		printStatus();
		break;
		
		case PRINT_COMMAND_LIST:
		printCommandList();
		break;

		case MODE_RESET:
		setMode(operatingMode);
		break;
		
		default:
		if (!leftLock){
			performAction(c, leftStrip);
		}
		if(!rightLock){
			performAction(c, rightStrip);
		}
	}
}


/**
* Enable input from the buzzers
*/
void enableBuzzers(){
	buzzersEnabled = true;
}


/**
* Disable input from the buzzers
*/
void disableBuzzers(){
	buzzersEnabled = false;
}


void printStatus(){
	//TODO Actually print the status
	// Mode, transition info, strobe info, colour?
}


void printCommandList(){
	//TODO Create the command list for printing
	
}


/**
* Perform a strip-specific action
*/
void performAction(char c, RgbStrip &strip){
	switch (c){
		case LIGHTS_OFF:
		strip.lightsOff();
		break;
		
		case RESET_STRIP:
		strip.initialiseStrip();
		break;
		
		
		case INCREASE_BRIGHTNESS:
		strip.increaseBrightness();
		break;
		
		case DECREASE_BRIGHTNESS:
		strip.decreaseBrightness();
		break;
		
		case LOW_BRIGHTNESS:
		strip.setLowBrightness();
		break;
		
		case FULL_BRIGHTNESS:
		strip.setFullBrightness();
		break;
		
		
		case ENABLE_STROBE:
		strip.enableStrobe();
		break;
		
		case DISABLE_STROBE:
		strip.disableStrobe();
		break;
		
		case INCREASE_STROBE_PERIOD:
		strip.increaseStrobePeriod();
		break;
		
		case DECREASE_STROBE_PERIOD:
		strip.decreaseStrobePeriod();
		break;
		
		
		case ENABLE_TRANSITIONS:
		strip.enableTransitions();
		break;
		
		case DISABLE_TRANSITIONS:
		strip.disableTransitions();
		break;
		
		case INCREASE_TRANSITION_PERIOD:
		strip.increaseTransitionPeriod();
		break;
		
		case DECREASE_TRANSITION_PERIOD:
		strip.decreaseTransitionPeriod();
		break;
		
		
		default:
		Serial.println("Command not recognized, enter 'H' to see command list");
	}
}


/**
* Set the colour of the unlocked strips
*/
void processColourCommand(char c){
	if (!leftLock){
		leftStrip.setColour(c);
	}
	
	if(!rightLock){
		rightStrip.setColour(c);
	}
}
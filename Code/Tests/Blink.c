// F_CPU tells util/delay.h our clock frequency
#define F_CPU 1000000UL // Clock frequency (1MHz)

// include the avr library
#include <avr/io.h>
#include <util/delay.h>

void delayms( long millis )
        {
        while ( millis )
        {
                _delay_ms( 1 );
                millis--;
        }
}


int main( void )
        {
        DDRD |= 0xff;                           // set ALL PORTD Outputs

        PORTD |= 0x00;                          // set PORTD pins, LOW


        while ( 1 )
        {

                PORTD |= 1 << PD1;              // LED on

                delayms( 500 );                 // delay ms

                PORTD &= ~( 1 << PD1 );         // LED off

                delayms( 500 );                 // delay ms

        }
        return 0;
}

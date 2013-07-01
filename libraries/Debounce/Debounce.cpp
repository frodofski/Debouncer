
/******************************************************************************************
 * Button debouncer for Arduino
 ******************************************************************************************
 * Copyright (c) Robert Bakker 2013
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 ******************************************************************************************
 * This library handles polling and debouncing of a button.
 * It is non-blocking code that executes in the background while your sketch
 * does it's thing.
 *****************************************************************************************/

#include "Debounce.h"
#include "Arduino.h"	// Has just about everything in it


/*Private #defines************************************************************************/

// Used by state machines
#define STANDBY 0
#define DELAYING 1
#define WAITING 2
#define IDLE 4


/*Declare Variables***********************************************************************/

struct buttonVars
{
	volatile uint8_t pin;

	volatile uint8_t state;
	volatile int8_t data;
	volatile uint8_t counter;

	volatile uint8_t pinState;
	volatile uint8_t pinStateOld;
};

// Initialize variables (start all state machines in STANDBY state)
buttonVars button = { 0,  STANDBY, 0, 0,  0, 0 };


/*Start of Functions**********************************************************************/

// This function sets up timer2 to trigger an ISR every 300 us.
// It also sets up the input pin(s).
void debounce_begin(uint8_t pin)
{
	button.pin = pin;

	pinMode(button.pin, INPUT);

	// Configure timer 2
	cli();					// Disable global interrupts

	TCCR2A = 0;				// Clear timer2's control registers
	TCCR2B = 0;
	TIMSK2 = 0;				// ...and interrupt mask register (just in case)
	TCNT2 = 0;				// Pre-load the timer to 0
	OCR2A = 149;			// Set output compare register to 149
	TCCR2A |= _BV(WGM21);	// Turn on CTC mode (Clear Timer on Compare match)
	TCCR2B |= 0b011;		// Set prescaler to 32 (starts timer) 
	TIMSK2 |= _BV(OCIE2A);	// Enable timer compare interrupt 

	sei();					// Re-enable global interrupts
}

int8_t button_data(void)
{
	return button.data;
}


/*ISR*************************************************************************************/

// This is the interrupt itself
// It is only active if button_begin() is called
// This polls the input pin(s) and debounces the incoming signals.
// "Debouncing" is done using a state machine.
ISR(TIMER2_COMPA_vect)
{
	// State machine for button
	switch(button.state)
	{
		case STANDBY:
			// Check its state first
			button.pinStateOld = digitalRead(button.pin);

			button.state = WAITING;
			break;

		case WAITING:
			// Look for change
			button.pinState = digitalRead(button.pin);

			if(button.pinState != button.pinStateOld)
			{
				button.state = DELAYING;
			}
			break;

		case DELAYING:
			// Wait for debounce period (~20ms)
			button.counter++;
			if(button.counter > DEBOUNCE_DELAY)
			{
				// Read it again
				button.data = digitalRead(button.pin);
				button.counter = 0;

				button.state = STANDBY;
			}
			break;
	}
}
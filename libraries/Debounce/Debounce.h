
/*****************************************************************************************/
/* Button debouncer for Arduino
/*****************************************************************************************/
/* Copyright (c) Robert Bakker 2013
/*
/* This file is free software; you can redistribute it and/or modify
/* it under the terms of either the GNU General Public License version 2
/* or the GNU Lesser General Public License version 2.1, both as
/* published by the Free Software Foundation.
/*****************************************************************************************/
/* This library handles polling and debouncing of a button.
/* It is non-blocking code that executes in the background while your sketch
/* does it's thing.
/*****************************************************************************************/

#ifndef Debounce_h		// Include guard
#define Debounce_h

#include "Arduino.h"	// Has just about everything in it

#define DEBOUNCE_DELAY 66

/*Global Variables************************************************************************/

// dust...


/*Functions Prototypes********************************************************************/

// This function sets up the library to debounce a button on pin.
void debounce_begin(uint8_t pin);

// Returns the debounced button state
int8_t button_data(void);


#endif	// Part of the include guard
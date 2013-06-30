
#include "Debounce.h"

const int buttonPin = 10;

int buttonState = 0;
int buttonStateOld = 0;

int ledState = 0;

void setup()
{
  pinMode(13, OUTPUT);
  debounce_begin(buttonPin); // State debouncing a button on buttonPin
}

void loop()
{
  buttonState = button_data(); // Check button state
  
  // Toggle pin 13 LED if the button's been pressed
  if(buttonState != buttonStateOld)
  {
    buttonStateOld = buttonState;
    if(buttonState == 1)
    {
      ledState = !ledState;
      digitalWrite(13, ledState);
    }
  }
}

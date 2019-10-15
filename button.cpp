//****************************************************************************// 
//  button.cpp
//
//  Andrew Jones
//
//  Description: This package defines methods for the Button class
//               which encapsulates the basic behavior of a discrete button
//               on the Arduino. 
// TBD: aoj replace polled buttons with interrupt driven for better responce time?
//      only three interrupts seem available, would need to ID which changed, only improvement is
//      skipping update when nothing is pressed, which may not be a gain
//****************************************************************************// 

#include "button.h"
#include "globals.h"

//****************************************************************************//
// Description: This function handles cyclical updates for a Button.  
//              It should operate most effectively if called at the fastest 
//              update rate available.
// Parameters:  None
// Return:      None
//****************************************************************************//
void Button::cylicalUpdate(void)
{

  //Store the old state
  bool prevState = currState;
  //Read the new state
  currState = digitalRead(pin);
  //check for press
  if ((prevState == LOW) && (currState == HIGH))
  {
    //begin counting down time
    debounceTimerMs = millis();
  }
  //When still pressed and the debounced state has not changed, check the timer
  else if (currState == HIGH && debouncedState == LOW) 
  {
    if ((millis() - debounceTimerMs) > debounceThreshold)
    {
      debouncedState = HIGH;
    }
  }
  else if(currState == LOW)
  {
    //don't debounce on rise, just clear it
    debouncedState = LOW;
    debounceTimerMs = 0;
  }
}

//****************************************************************************// 
//  button.h
//
//  Andrew Jones
//
//  Description: This package defines a class for the Button
//               which encapsulates the basic behavior of a polled discrete 
//               button on the Arduino. 
//****************************************************************************//

#ifndef _BUTTON_H   //multi include guard 
#define _BUTTON_H   

#include <Arduino.h>

class Button
{
  private:
    //Hardware pin associated with this button
    byte pin = 0;
    
    //The non-debounced state of the button
    bool currState;
    
    //Timer that holds how long the non-debounced state has been 'down'
    unsigned long debounceTimerMs;
    
    bool debouncedState;
    
    const unsigned long debounceThreshold = (unsigned long)15; //TBD aoj reconsider threshold based on performance?
  
  public:
    //Simple constructor, provide the pin for this button
    Button (byte inPin): pin(inPin), currState(LOW), debouncedState(LOW), debounceTimerMs(0) {pinMode(inPin, INPUT);}

    //Update function that checks the current state of the button and processes the debounce time that it has been pressed
    //It should be called at a relatively fast rate for the best responce time
    void cylicalUpdate(void);

    //Returns the current debounced state of the button
    bool isDown(void) {return (debouncedState == HIGH);} 
};


#endif // _BUTTON_H   

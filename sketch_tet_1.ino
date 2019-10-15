//****************************************************************************// 
//  main tetris sketch file
//
//  Andrew Jones
//
//  Description: This main sketch for the Tetris project
//               Reverse engineers from observation the Tetris game to opperate on
//               an arduino with a 1 inch i2c display.
//               This file will contain the main loop() and update() functions
//               required by the arduino libraries.
//
//tbd aoj remove all but loop and update into specific modules and wrap debugging code
//in #defines for deactivation as functionality is added.
//****************************************************************************// 

#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>

#include <Adafruit_SSD1306.h>
#include <splash.h>

#include "minos.h"
#include "button.h"
#include "globals.h"
#include "grid.h"

// OLED display TWI address
#define OLED_ADDR   0x3C

// reset pin not used on 4-pin OLED module
Adafruit_SSD1306 display(-1);  // -1 = no reset pin

//Set up inputs
Button buttonRight(RIGHT_PIN);
Button buttonLeft(LEFT_PIN);
Button buttonDown(DOWN_PIN);
Button buttonRotate(ROTATE_PIN);
Button buttonDrop(DROP_PIN);

//Create objects for the game buffers
Grid * currTetraminoGrid = new Grid();
Grid * displayedTetraminoGrid = new Grid();

//aoj debug remove
Zig * myZig = new Zig(1, 1);

//Score
//aoj tbd anything to be gained by putting this in a class?
unsigned int u16CurrentScore = 0;
byte byCurrentDifficulty = 1;

//Procedure to draw the current state of the abstract grid to the display
//TBD aoj move this into the grid package?  Or should the display a seperate module as it is not abstract?
//TBD aoj remove magic numbers and clean up comments in new location
void DisplayGrid(void)
{
  //Start drawing in the upper left corner of the frame
  static byte startX = 2;
  static byte startY = 12;
  unsigned int mask = 0;
  unsigned int displayedRow = 0;
  unsigned int currentRow = 0;

  for (int wordIndex = 0; wordIndex < MAX_DISP_GRID_Y; wordIndex++)
  {
    //Grab the data for this row
    currentRow = currTetraminoGrid->GetRow(wordIndex + 2); //skip the top two and bottom two 'offscreen' rows
    currentRow >>= 2; //skip the first two offscreen bits 
    displayedRow = displayedTetraminoGrid->GetRow(wordIndex);

    for (int bitIndex = 0; bitIndex < MAX_DISP_GRID_X; bitIndex++)
    {
      //create a mask to read the correct bit from this word
      mask = 1 << bitIndex;
      //when an additive change is detected, draw a white rect
      if (((displayedRow & mask) == 0) &&
          ((currentRow & mask) > 0))
      {
        //This block should be drawn white
        display.drawRect((startX + (bitIndex * 6) ),(startY + (wordIndex * 6) ), 4, 4, WHITE);
        displayedRow |= mask;
        displayedTetraminoGrid->SetRow(wordIndex, displayedRow);
      }
      //when a subtractive change is detected, draw a black rect
      if (((displayedRow & mask) > 0) &&
          ((currentRow & mask) == 0))
      {
        //This block should be drawn black
        display.drawRect((startX + (bitIndex * 6) ),(startY + (wordIndex * 6) ), 4, 4, BLACK);
        displayedRow &= ~mask;
        displayedTetraminoGrid->SetRow(wordIndex, displayedRow);
      }
      //tbd AOJ optimize this down to a tertiary?
    }
  }
}

void DisplayScore(void)
{
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0,0);      // Start at top-left corner
  display.println(u16CurrentScore, DEC);
}


//timers for tracking cycle
unsigned long lastMillis5, lastMillis25, lastMillis100;


void PrintGrid(void)
{
  Serial.println(F("Printall"));
  for (int i = 0; i<GRID_WORD_MAX ;i++)
  {
     Serial.println(currTetraminoGrid->GetRow(i), BIN); //aoj debug remove 
  }
}

void setup() {
  
  //Set up serial port for communication with PC
  Serial.begin(9600);

  // initialize display
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) 
  { 
    Serial.println(F("SSD1306 allocation failed"));
    while (true); // Don't proceed, loop forever
  }
  else
  {
    //clear the display
    display.clearDisplay();
    display.display();
  }

  // Set up the Text display
  display.setTextSize(2);
  display.setTextColor(WHITE);

  // Set up the display orientation
  display.setRotation(TWOSEVENTY_ROTATE);
      
  // Display fixed elements
    //The Frame
  display.drawRect(FRAME_XO, FRAME_YO, FRAME_WIDTH, FRAME_HEIGHT, WHITE);
  //Add the invisible frame to the abstract game grid
  for(int i=0;i<GRID_WORD_MAX;i++)
    currTetraminoGrid->SetRow(i, 0x1002);
    
  currTetraminoGrid->SetRow(MAX_DISP_GRID_Y + 2, 0x1FFC); //aoj move this to display grid setup?
  currTetraminoGrid->SetRow(MAX_DISP_GRID_Y + 1, 0x1FFC); //aoj move this to display grid setup?
  currTetraminoGrid->SetRow(MAX_DISP_GRID_Y + 0, 0x1FFC); //aoj move this to display grid setup?
  currTetraminoGrid->SetRow(MAX_DISP_GRID_Y - 1, 0x1FFC); //aoj move this to display grid setup?
  PrintGrid();
  Serial.println(F("frame drawn")); //aoj debug remove

    // update display with all of the above graphic changes
  display.display(); 

myZig->AddToGrid(currTetraminoGrid);//aoj debug remove

Serial.println(F("setup done")); //aoj debug remove
}

void do5msTask(void)
{
  //Perform button cylical maintenance when the pin interrupt has triggered
    buttonRight.cylicalUpdate();
    buttonLeft.cylicalUpdate();
    buttonDown.cylicalUpdate();
    buttonRotate.cylicalUpdate();
    buttonDrop.cylicalUpdate();

    //Process the auto-drop from gravity
  //  myZig->ApplyGravity(currTetraminoGrid, byCurrentDifficulty);
}

void do25msTask(void)
{
    //Check for debounced button presses
    if (buttonRight.isDown())
    {
      Serial.println(F("button R down")); //aoj debug remove
      myZig->MoveRight(currTetraminoGrid);
    }
    if (buttonLeft.isDown())
    {
      Serial.println(F("button L down")); //aoj debug remove
      myZig->MoveLeft(currTetraminoGrid);
    }
    if (buttonDown.isDown())
    {
      Serial.println(F("button D down")); //aoj debug remove
      myZig->MoveDown(currTetraminoGrid);
    }
    if (buttonRotate.isDown())
    {
      Serial.println(F("button Ro down")); //aoj debug remove
      myZig->Rotate(currTetraminoGrid);
    }
    if (buttonDrop.isDown()){Serial.println(F("button Dr down"));} //aoj debug remove 
}

void do100msTask(void)
{
  //Check for newly scored lines
  u16CurrentScore += currTetraminoGrid->CheckForScoring(byCurrentDifficulty);

  PrintGrid();
  DisplayGrid();
  DisplayScore();
      // update display with all of the above graphic changes
    display.display(); 
 //aoj end test
}


void loop() 
{
//determine task
unsigned long currentMillis = millis(); //aoj will roll over after 50 days?

if ((unsigned long) (currentMillis - lastMillis5) >=  5 )
{
  do5msTask();
  lastMillis5 = currentMillis;
}
if ( (unsigned long) (currentMillis - lastMillis25) >= 25 )
{
  do25msTask();
  lastMillis25 = currentMillis;
}
if ( (unsigned long) (currentMillis - lastMillis100) >= 100 ) //tbd aoj constants on timing wrong, investigate
{
  do100msTask();
  lastMillis100 = currentMillis;
}
  


}

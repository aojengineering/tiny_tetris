//****************************************************************************// 
//  minos.cpp
//
//  Andrew Jones
//
//  Description: This package defines methods for the Tetramino class
//               which encapsulates one of the falling game pieces on the 
//               board.
//****************************************************************************// 

#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>

#include <Adafruit_SSD1306.h>
#include <splash.h>

#include "minos.h"
#include "globals.h"

//****************************************************************************//
// Description: This function adds this Tetramino to the grid.
// Parameters:  Grid - the abstract grid to add the 'mino to
// Return:      None
//****************************************************************************//
void Tetramino::AddToGrid(Grid * grid)
{
   unsigned int tempRow = 0;

   //Add it to the center of the top row
   locY = 2;
   locX = 5;

   for (int i = 0; i < NUM_ROW_IN_MINO; i++)
   {
    //aoj add collision detection?
    tempRow = grid->GetRow(locY + i);
    unsigned int mask = (0xf << (i * NUM_ROW_IN_MINO));
    tempRow |= (((pattern[orientation] & mask) >> (i * NUM_ROW_IN_MINO)) << locX);
    grid->SetRow(locY + i, tempRow);
   }
}

//****************************************************************************//
// Description: This function changes the location of this Tetramino in the grid.
//              Before making the change it verifies no collision with existing 
//              blocks is detected.
// Assumptions: Assumes the Tetramino exists in it's current location.
// Parameters:  Grid - the abstract grid
//              inLocX - the new X coordinate
//              inLocY - the new Y coordinate
//              inOrientation - the new orientation of the Tetramino
// Return:      None
//TBD aoj collision detection not viable on edges, reconsider how grid is represented
//****************************************************************************//
void Tetramino::Change(byte inLocX, byte inLocY, ORIENTATION inOrientation, Grid * grid)
{
  unsigned int tempRow = 0;
  unsigned int mask = 0;
  bool collisionDetected = false;

  //Verify that a change is actually occuring
  if ((locY != inLocY) || (locX != inLocX) || (inOrientation != orientation))
  {
  
    for (int i = 0; i < NUM_ROW_IN_MINO; i++)
    {
      //clear bits from old location
      tempRow = grid->GetRow(locY + i);
#ifdef DEBUG_MOVEMENT
      Serial.println(F("grabbed"));//aoj debug remove
      Serial.println(tempRow); //aoj debug remove
#endif //DEBUG_MOVEMENT
      mask = (0xf << (i * NUM_ROW_IN_MINO)); 
      tempRow &= ~(((pattern[orientation] & mask) >> (i * NUM_ROW_IN_MINO)) << locX);
#ifdef DEBUG_MOVEMENT
      Serial.println(F("clear old"));//aoj debug remove
      Serial.println(tempRow);  //aoj debug remove
#endif //DEBUG_MOVEMENT
      grid->SetRow(locY + i, tempRow); 
  
    }

    //Detect collisions in new position
    collisionDetected = CollisionIsDetected(inLocX, inLocY, inOrientation, grid);

    //move should occur if collision is not detected
    if (!(collisionDetected))
    {
      for (int i = 0; i < NUM_ROW_IN_MINO; i++)
      { 
        //set bits in new location 
        tempRow = grid->GetRow(inLocY + i);
#ifdef DEBUG_MOVEMENT
        Serial.println(F("moved to"));//aoj debug remove
#endif //DEBUG_MOVEMENT
        mask = (0xf << (i * NUM_ROW_IN_MINO));
        tempRow |= (((pattern[inOrientation] & mask) >> (i * NUM_ROW_IN_MINO)) << inLocX);
#ifdef DEBUG_MOVEMENT
        Serial.println(tempRow);//aoj debug remove
#endif //DEBUG_MOVEMENT
        grid->SetRow(inLocY + i, tempRow);
      }
      
      //Then update current locations
      locY = inLocY;
      locX = inLocX;
      orientation = inOrientation;
    }
    else //put it back in the dumb old spot
    {
      for (int i = 0; i < NUM_ROW_IN_MINO; i++)
      { 
        //set bits in new location 
        tempRow = grid->GetRow(locY + i);
#ifdef DEBUG_MOVEMENT
        Serial.println(F("no move"));//aoj debug remove
#endif //DEBUG_MOVEMENT
        mask = (0xf << (i * NUM_ROW_IN_MINO));
        tempRow |= (((pattern[inOrientation] & mask) >> (i * NUM_ROW_IN_MINO)) << locX);
        grid->SetRow(locY + i, tempRow);
      }    
    }
  }
   
}

//****************************************************************************//
// Description: This function determines if a new location for this Tetramino will
//              cause it to collide with an existing block or with the frame.
// Parameters:  Grid - the abstract grid 
//              inLocX - the new X coordinate
//              inLocY - the new Y coordinate
//              inOrientation - the new orientation of the Tetramino
// Return:      True if there is a collision
//****************************************************************************//
bool Tetramino::CollisionIsDetected(byte inLocX, byte inLocY, ORIENTATION inOrientation, Grid * grid)
{
  bool retval = false;
  unsigned int mask = 0;
  bool thisBitSet = false;
  unsigned int tempRow = 0;

  //check each active cube in the new position
  //scan each pattern nibble
  for (int i = 0; i < NUM_ROW_IN_MINO && (retval == false); i++)
  { 
    //get bits in new location 
    tempRow = grid->GetRow(inLocY + i);
#ifdef DEBUG_COLLISION
    Serial.println(F("Collision detecting")); //aoj debug remove
    Serial.println(tempRow); //aoj debug remove
#endif //DEBUG_COLLISION
    //tbd aoj could skip blank nibs
    //scan each bit in nibble
    for (int c = 0; c < NUM_ROW_IN_MINO && (retval == false); c++) //actually, counting columns
    {
       //Determine if this cube is active in pattern
       mask = (0x1 << c + (i * NUM_ROW_IN_MINO));
#ifdef DEBUG_COLLISION
       Serial.println(mask); //aoj debug remove
#endif //DEBUG_COLLISION           
       thisBitSet = ((pattern[inOrientation] & mask) == mask);
#ifdef DEBUG_COLLISION
       Serial.println(thisBitSet); //aoj debug remove
#endif //DEBUG_COLLISION

       //Determine if new location collides
       if (thisBitSet)
       {
          mask = (0x1 << (inLocX + c));
          retval = ((tempRow & mask) == mask);
#ifdef DEBUG_COLLISION
          Serial.println(mask);  //aoj debug remove   
#endif //DEBUG_COLLISION        
       }
    }
  }
  return retval;
}


//Simple public function to move the Tetramino one block to the Right
void Tetramino::MoveRight(Grid * grid)
{
  Change((locX + 1), locY, orientation, grid);
}

//Simple public function to move the Tetramino one block to the Left
void Tetramino::MoveLeft(Grid * grid)
{
  Change((locX - 1), locY, orientation, grid);
}

//Simple public function to move the Tetramino one block Downwards
void Tetramino::MoveDown(Grid * grid)
{
  Change(locX, (locY + 1), orientation, grid);
}

//aoj tbd comment
void Tetramino::ApplyGravity(Grid * grid, byte difficulty)
{
  static unsigned long gravTimer;
  unsigned long prevGravTimer = 0;

  gravTimer = millis(); 
  //look up the minimum delay for this difficulty level
  if ((unsigned long) (gravTimer - prevGravTimer) >=  ulayGravityLookup[difficulty] )
  {
    MoveDown(grid);
    prevGravTimer = gravTimer;
    Serial.println(F("grav"));  //aoj debug remove 
  }
  
  
}

//Simple public function to rotate the Tetramino in place 90 degrees to the right
void Tetramino::Rotate(Grid * grid)
{
   ORIENTATION newOrientation = ORIENTATION(orientation + 1);

   //Check for rollover
   if (newOrientation >= MAX_ORIENTATIONS)
      newOrientation = ORIENTATION_UP;
  Change(locX, locY, newOrientation, grid);
}

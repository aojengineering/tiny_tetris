//****************************************************************************// 
//  grid.cpp
//
//  Andrew Jones
//
//  Description: This package defines methods for the Grid class
//               which encapsulates access to the abstract game grid that an 
//               individual tetramino block can occupy.
//****************************************************************************// 

#include "grid.h"
#include "globals.h"

//simple access for one row of blocks
int Grid::GetRow(byte index)
{
  int retVal = 0;
  if (index < GRID_WORD_MAX)
     retVal = myGrid[index];

  return retVal;
}

//simple set for one row of blocks
void Grid::SetRow(byte index, int rowVal)
{
  if (index < GRID_WORD_MAX)
     myGrid[index] = rowVal;
}

//****************************************************************************//
// Description: This function checks every row on the abstract gameboard to
//              detect any rows that are completely filled.  If any are found, 
//              they are removed from the grid and those above moved down to fill.
//              Scoring is perfomred for each completed row as follows:
//                 10x + 5b where x is the difficulty level 
//                 and b is the Bonus for each line after the first that is completed.
// Parameters:  
//              difficulty - the current difficulty level, used to calculate the value
//                           of newly completed rows.
// Return:      Amount of points to add to total for this cycle.
//****************************************************************************//
int Grid::CheckForScoring(byte difficulty)
{
  int retScore = 0;
  byte rowCountToScore = 0;
  byte rowNumbersToScore[4] = {}; //Should never be more than 4 completed at once.. I think 

  //scan all displayed rows from the bottom up, skipping the 2 invisible rows at the top and bottom
  for (byte i = (GRID_WORD_MAX - 3); (i > 2) && (rowCountToScore < 4); i--)
  {
    int mask = 0xFFC; //mask the entire visible portion of a row
    if ((GetRow(i) & mask) == mask)
       rowNumbersToScore[rowCountToScore++] = i;    
  }
  
#ifdef DEBUG_SCORE
   Serial.println(F("lines to score"));//aoj debug remove
   Serial.println(rowCountToScore);//aoj debug remove
#endif //DEBUG_SCORE

  if (rowCountToScore > 0)
  {
    //Calculate the score addition
    retScore = (rowCountToScore * ((10 * difficulty) + ((rowCountToScore - 1) * 5)));
    //aoj tbd check scale for rollover?
     
    //aoj TBD flash these rows?
    //Remove the rows in question and pack the rows above
    for (byte completeIndex = rowCountToScore; completeIndex > 0; completeIndex--)
    {   
       //slide down all rows above the last scored line in our list   
       for (byte rowIndex = rowNumbersToScore[completeIndex - 1]; rowIndex > 2; rowIndex--)
       {
          SetRow(rowIndex, GetRow(rowIndex - 1));
       }
    }    
  }

#ifdef DEBUG_SCORE
   Serial.println(F("score return"));//aoj debug remove
   Serial.println(retScore);//aoj debug remove
#endif //DEBUG_SCORE
  return retScore;
}

void Grid::AddRandomNextTetramino(void)
{

  if (ptrNextTetramino != null)
  {
    MoverNextMinoToActive();
  }

  //Next should be newly empty or was previously
  if (ptrNextTetramino == null)
  {
    byte randTet = random(0, 6) //7 types of tetraminos
    switch (randTet)
    {
       case 0:
          ptrNextTetramino = new Zig(NEW_MINO_X, NEW_MINO_Y);
          break;
       case 1:
          ptrNextTetramino = new Zag(NEW_MINO_X, NEW_MINO_Y);
          break;
       case 2:
          ptrNextTetramino = new Tee(NEW_MINO_X, NEW_MINO_Y);
          break;
       case 3:
          ptrNextTetramino = new Square(NEW_MINO_X, NEW_MINO_Y);
          break;
       case 4:
          ptrNextTetramino = new LeftL(NEW_MINO_X, NEW_MINO_Y);
          break;
       case 5:
          ptrNextTetramino = new RightL(NEW_MINO_X, NEW_MINO_Y);
          break;
       case 6:
          ptrNextTetramino = new Zig(NEW_MINO_X, NEW_MINO_Y);
          break;
       default:
          //something went wrong
          ptrNextTetramino = new TheFinisher(NEW_MINO_X, NEW_MINO_Y);
          #ifdef DEBUG_ERR
             Serial.println(F("random fail"));//aoj debug remove
             Serial.println(randTet);//aoj debug remove
          #endif //DEBUG_ERR
          break;
  }
}

void Grid::MoverNextMinoToActive(void)
{
  //trust but verify that Active is currently empty
  if ((fTetraminoActive == false) && (ptrActiveTetramino == null))
  {
    ptrActiveTetramino = ptrNextTetramino;
    fTetraminoActive = true;
    AddActiveTetraminoToGrid();
    ptrNextTetramino = null;
  }
}

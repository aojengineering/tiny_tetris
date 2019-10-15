//****************************************************************************// 
//  grid.h
//
//  Andrew Jones
//
//  Description: This package defines a class for the grid
//               which encapsulates access to the abstract game grid that an 
//               individual tetramino block can occupy.
//****************************************************************************//

#ifndef _GRID_H   //multi include guard 
#define _GRID_H    

#include "globals.h"
#include <Arduino.h>
#include "mino.h"

//define the play grid
class Grid
{
  public:
  Grid(){};
  int GetRow(byte index);
  void SetRow(byte index, int rowVal);
  int CheckForScoring(byte difficulty); 
  void AddRandomNextTetramino(void);
  void MoverNextMinoToActive(void);

  private:
  //The grid consists of 24 16 bit values, each bit is set when it is occupied by a block
  //the final 2 bits of each row are currently unused
  unsigned int myGrid[GRID_WORD_MAX] = {};

  //In order to seperately track tetraminos affected by gravity/movement they are their own objects
  boolean fTetraminoActive = false;
  Tetramino * ptrActiveTetramino = null;
  Tetramino * ptrNextTetramino = null;
};

#endif // _GRID_H  

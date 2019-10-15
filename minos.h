//****************************************************************************// 
//  minos.h
//
//  Andrew Jones
//
//  Description: This package defines the parent Tetramino class as well as each 
//               child class that represents particular configurations of Tetraminos.
//               which encapsulates one of the falling game pieces on the 
//               board.
//****************************************************************************//

#ifndef _MINOS_H   //multi include guard 
#define _MINOS_H    

#include "grid.h"

//define a tetramino 
class Tetramino
{
  protected:
  // Represent 4x4 array of cubes as 1 16 bit value
  //  OOOO 
  //  OOO* 
  //  **** 
  //  OOOO pattern[0] = 0x01f0
  //Each value represents one orientation of the Tetramino stored in an array

  unsigned int pattern[MAX_ORIENTATIONS] = {};
  
  // record the location of the upper left cube in the 20x10 grid of possible cubes
  byte locX = 0;
  byte locY = 0;
  
  ORIENTATION orientation = ORIENTATION_UP;

  public:
  Tetramino(byte inLocX, byte inLocY) : locX(inLocX), locY(inLocY){};
  virtual void AddToGrid(Grid * grid);
  virtual void MoveRight(Grid * grid);
  virtual void Change(byte inLocX, byte inLocY, ORIENTATION inOrientation, Grid * grid);  //tbd aoj make private?
  virtual void MoveLeft(Grid * grid);
  virtual void MoveDown(Grid * grid);//aoj add variable speed of movement?
  virtual void Rotate(Grid * grid);
  virtual bool CollisionIsDetected(byte inLocX, byte inLocY, ORIENTATION inOrientation, Grid * grid);
  virtual void ApplyGravity(Grid * grid, byte difficulty);
  
};

//Defines a subclass of Tetramino for a particular shape
class Zig : public Tetramino
{
  private:
  public:
  Zig (byte inLocX, byte inLocY): Tetramino(inLocX, inLocY)
  {
    pattern[ORIENTATION_UP] = 0x0630;
    pattern[ORIENTATION_RIGHT] = 0x2640;
    pattern[ORIENTATION_DOWN] = 0x0630;
    pattern[ORIENTATION_LEFT] = 0x2640;
  };
};

//Defines a subclass of Tetramino for a particular shape
class Zag : public Tetramino
{
  private:
  public:
  Zag (byte inLocX, byte inLocY): Tetramino(inLocX, inLocY)
  { 
     pattern[ORIENTATION_UP] = 0x06C0;
     pattern[ORIENTATION_RIGHT] = 0x4620;
     pattern[ORIENTATION_DOWN] = 0x06C0;
     pattern[ORIENTATION_LEFT] = 0x4620;
  };
};

//Defines a subclass of Tetramino for a particular shape
class Tee : public Tetramino
{
  private:
  public:
  Tee (byte inLocX, byte inLocY): Tetramino(inLocX, inLocY)
  {
     pattern[ORIENTATION_UP] = 0x0720;
     pattern[ORIENTATION_RIGHT] = 0x2620;
     pattern[ORIENTATION_DOWN] = 0x0270;
     pattern[ORIENTATION_LEFT] = 0x4640;
  };
};

//Defines a subclass of Tetramino for a particular shape
class Square : public Tetramino
{
  private:
  public:
  Square (byte inLocX, byte inLocY): Tetramino(inLocX, inLocY)
  {
     pattern[ORIENTATION_UP] = 0x0660;
     pattern[ORIENTATION_RIGHT] = 0x0660;
     pattern[ORIENTATION_DOWN] = 0x0660;
     pattern[ORIENTATION_LEFT] = 0x0660;
  };
};

//Defines a subclass of Tetramino for a particular shape
class LeftL : public Tetramino
{
  private:
  public:
  LeftL (byte inLocX, byte inLocY): Tetramino(inLocX, inLocY)
  {
     pattern[ORIENTATION_UP] = 0x0470;
     pattern[ORIENTATION_RIGHT] = 0x0644;
     pattern[ORIENTATION_DOWN] = 0x0e20;
     pattern[ORIENTATION_LEFT] = 0x2260;
  };
};

//Defines a subclass of Tetramino for a particular shape
class RightL : public Tetramino
{
  private:
  public:
  RightL (byte inLocX, byte inLocY): Tetramino(inLocX, inLocY)
  {
     pattern[ORIENTATION_UP] = 0x02e0;
     pattern[ORIENTATION_RIGHT] = 0x4460;
     pattern[ORIENTATION_DOWN] = 0x0e80;
     pattern[ORIENTATION_LEFT] = 0x6220;
  };
};

//Defines a subclass of Tetramino for a particular shape
class TheFinisher : public Tetramino
{
  private:
  public:
  TheFinisher (byte inLocX, byte inLocY): Tetramino(inLocX, inLocY)
  {
     pattern[ORIENTATION_UP] = 0x0F00;
     pattern[ORIENTATION_RIGHT] = 0x2222;
     pattern[ORIENTATION_DOWN] = 0x0F00;
     pattern[ORIENTATION_LEFT] = 0x2222;
  };
};

#endif // _MINOS_H    

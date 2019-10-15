//****************************************************************************// 
//  globals.h
//
//  Andrew Jones
//
//  Description: This package defines constants and enumerations used throughout  
//               the project.
//****************************************************************************//

#ifndef _GLOBALS_H   //multi include guard 
#define _GLOBALS_H    

//Debugging sentinels
//#define DEBUG_MOVEMENT 1
//#define DEBUG_COLLISION 1
#define DEBUG_SCORE 1
#define DEBUG_ERR 1

// The rotation parameter needed to orient our display vertically
#define TWOSEVENTY_ROTATE 0x3

// Define fixed graphic elements
#define FRAME_XO 0
#define FRAME_YO 10
#define FRAME_WIDTH 64
#define FRAME_HEIGHT 118

// Establish the game grid representing a 20x10 bitfield as 20 16bit values
// This grid will be displayed on an update cycle
#define GRID_WORD_MAX 24
#define GRID_BIT_MAX 14 //only first 14 bits are used
//aoj optimization: pack words better?  Or would access complexity not be worth the space saving?

#define MAX_DISP_GRID_X 10 //grid width in blocks
#define MAX_DISP_GRID_Y 20 //grid height in blocks

#define NUM_ROW_IN_MINO 4 //the number of block rows in a Tetramino

#define NEW_MINO_X 5
#define NEW_MINO_Y 2

//Define input pins
#define RIGHT_PIN 8
#define DOWN_PIN 12
#define LEFT_PIN 11
#define ROTATE_PIN 10
#define DROP_PIN 9

//The current orientation of a Tetramino
enum ORIENTATION
{
  ORIENTATION_UP = 0,
  ORIENTATION_RIGHT,
  ORIENTATION_DOWN,
  ORIENTATION_LEFT,
  MAX_ORIENTATIONS
};

//game rule definitions
#define MAX_DIFFICULTY_LEVELS 6

//Lookup table for gravity speeds by difficulty level
const unsigned long ulayGravityLookup[MAX_DIFFICULTY_LEVELS] =
{100, 80, 70, 60, 50, 30}; //aoj TBD arbitrary, scale these

#endif // _GLOBALS_H  

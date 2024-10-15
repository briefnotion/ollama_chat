// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: definitions.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define RASFLED

// Debug Counters
#define DEBUG_COUNTER false

// -------------------------------------------------------------------------------------
// DEFINITIONS
// -------------------------------------------------------------------------------------

#define Revision "0.005_241014"
#define  QACODE  "Z782"

// -------------------------------------------------------------------------------------
// CONSOLE AND INTEFACE SETUP

// Variable Redraw Rates
// 60 - 15 fps
// 30 - 30 fps
// 15 - 60 fps
//#define SCREENUPDATEDELAY 30  // in milliseconds 
#define OLLAMA_SLEEP_TIMER_DELAY  15  // This will be in frames per second (fps)
#define TEST_THREAD_TIMER_DELAY   7   // This will be in frames per second (fps)

#define TEST_MODE           false




/*
#define TEST_DATA   false
*/

// StupidRandom Parameters
#define StuRNDsize              100


#endif
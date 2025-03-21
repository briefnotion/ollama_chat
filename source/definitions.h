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
//#define DEBUG_COUNTER false

// -------------------------------------------------------------------------------------
// DEFINITIONS
// -------------------------------------------------------------------------------------

#define Revision "0.052-250322"
#define  QACODE  "Z782"

// -------------------------------------------------------------------------------------
// CONSOLE AND INTEFACE SETUP

// Variable Redraw Rates
// 60 - 15 fps
// 30 - 30 fps
// 15 - 60 fps
//#define SCREENUPDATEDELAY 30  // in milliseconds 
#define SCREENIO_SLEEP_TIMER_DELAY    30  // This will be in frames per second (fps)
#define OLLAMA_SLEEP_TIMER_DELAY      60  // This will be in frames per second (fps)
#define EMBEDDING_SLEEP_TIMER_DELAY   60  // This will be in frames per second (fps)
#define THOUGHTS_SLEEP_TIMER_DELAY    60  // This will be in frames per second (fps)

#define TEST_MODE           false

#define INTEFACE_NON_BLOCKING_MODE true

/*
#define TEST_DATA   false
*/


// Definitions for VectorDB API Collections
#define VECTORDB_API_COLLECTION_DOCUMEMTATION "documentation"
#define VECTORDB_API_COLLECTION_CHAT_HISTORY  "chat_history"

// Auxilary Definitions
// StupidRandom Parameters
#define StuRNDsize              100
#define DUMP_DIRECTORY          "/home/briefn/chat_api/dump/"
#define EXCHANGE_DIRECTORY      "/home/briefn/chat_api/exchange/"

#endif
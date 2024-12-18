#ifndef SYSTEM_H
#define SYSTEM_H

//#include <future>

/*
  ollama-hpp is a header-only C++ implementation of a state-of-the-art AI model.
  This library was created by Jonathan Monty and is available under the MIT License.
  For more details visit: https://github.com/jmont-dev/ollama-hpp
*/

//#include <nlohmann/json.hpp>

#include "stringthings.h"

#include "definitions.h"
#include "helper.h"
#include "fled_time.h"
#include "tty_io.h"
#include "threading.h"
#include "remember.h"

using namespace std;

// -------------------------------------------------------------------------------------

// Contains all varibles to be access by the main and provides a communications 
//  channel between them.

// -------------------------------------------------------------------------------------

class SYSTEM
{
  public:

  // Main Program Sleep Timers
  TIMED_IS_READY    SCREENIO_SLEEP_TIMER;
  TIMED_IS_READY    OLLAMA_SLEEP_TIMER;
  TIMED_IS_READY    EMBEDDING_SLEEP_TIMER;
  TIMED_IS_READY    THOUGHTS_SLEEP_TIMER;

  // I/O
  TTY_INPUT         INPUT;
  
  TTY_OUTPUT_FOCUS  OUTPUT_FOCUS;
  
  TTY_OUTPUT        OUTPUT_CLOCK;
  TTY_OUTPUT        OUTPUT_INPUT;
  TTY_OUTPUT        OUTPUT_OLLAMA_RESPONSE;

  // Program Clock
  FLED_TIME         PROGRAM_TIME;

  // Remember
  REMEMBER          MEMORY;
};

// ------------------------------------------------------------------------- //


#endif // SYSTEM_H
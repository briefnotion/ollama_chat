#ifndef SYSTEM_H
#define SYSTEM_H

#include <future>

/*
  ollama-hpp is a header-only C++ implementation of a state-of-the-art AI model.
  This library was created by Jonathan Monty and is available under the MIT License.
  For more details visit: https://github.com/jmont-dev/ollama-hpp
*/
#include "ollama.hpp"

#include "helper.h"
//#include "stringthings.h"
#include "fled_time.h"
#include "threading.h"
#include "ollama_api.h"

using namespace std;

// -------------------------------------------------------------------------------------


// -------------------------------------------------------------------------------------

class SYSTEM
{
  public:

  FLED_TIME        PROGRAM_TIME;

  OLLAMA_API OLLAMA_SYSTEM;
  THREADING_INFO OLLAMA_RESPONSE_THREAD;

};

// ------------------------------------------------------------------------- //


#endif // SYSTEM_H
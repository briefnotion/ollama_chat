#ifndef SYSTEM_H
#define SYSTEM_H

/*
  ollama-hpp is a header-only C++ implementation of a state-of-the-art AI model.
  This library was created by Jonathan Monty and is available under the MIT License.
  For more details visit: https://github.com/jmont-dev/ollama-hpp
*/
#include "ollama.hpp"

#include "helper.h"
//#include "stringthings.h"
#include "fled_time.h"

using namespace std;

// -------------------------------------------------------------------------------------


// -------------------------------------------------------------------------------------

class SYSTEM
{
  public:

  FLED_TIME        PROGRAM_TIME;
  //COMMAND_THREAD  COMMAND_THREADS;

  Ollama OLLAMA;
  ollama::options options;
  ollama::response responce;

  bool FULL_RESPONSE_RECEIVED = 0;
  // 0 wait
  // 1 sent
  // 3 responce received.
  
  vector<string> OLLAMA_MODELS_LIST;
  vector<string> OLLAMA_MODELS_RUNNING;

};

#endif // SYSTEM_H
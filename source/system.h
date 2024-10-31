#ifndef SYSTEM_H
#define SYSTEM_H

//#include <future>

/*
  ollama-hpp is a header-only C++ implementation of a state-of-the-art AI model.
  This library was created by Jonathan Monty and is available under the MIT License.
  For more details visit: https://github.com/jmont-dev/ollama-hpp
*/

/*/
#include <nlohmann/json.hpp>

//#include "stringthings.h"
*/

#include "definitions.h"
#include "helper.h"
#include "fled_time.h"
#include "tty_io.h"
#include "threading.h"

//#include "ollama_api.h"

#include "chromadb_api.h"

using namespace std;

// -------------------------------------------------------------------------------------

// Contains all varibles to be access by the main and provides a communications 
//  channel between them.

// -------------------------------------------------------------------------------------

class SYSTEM
{
  public:

  // I/O
  TTY_INPUT INPUT;
  
  TTY_OUTPUT_FOCUS  OUTPUT_FOCUS;
  TTY_OUTPUT        OUTPUT_INPUT;
  TTY_OUTPUT        OUTPUT_OLLAMA_RESPONSE;

  // Program Clock
  FLED_TIME        PROGRAM_TIME;

  // Ollama System
  //OLLAMA_API      OLLAMA_SYSTEM;
  THREADING_INFO  OLLAMA_RESPONSE_THREAD;

  // ChromaDB System
  CHROMADB_API    CHROMADB_SYSTEM;

};

// ------------------------------------------------------------------------- //


#endif // SYSTEM_H
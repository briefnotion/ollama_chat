#ifndef SYSTEM_H
#define SYSTEM_H

/*
  ollama-hpp is a header-only C++ implementation of a state-of-the-art AI model.
  This library was created by Jonathan Monty and is available under the MIT License.
  For more details visit: https://github.com/jmont-dev/ollama-hpp
*/
#include "ollama.hpp"

using namespace std;

class SYSTEM
{
  public:

  Ollama OLLAMA;
  
  vector<string> OLLAMA_MODELS_LIST;
  vector<string> OLLAMA_MODELS_RUNNING;

};

#endif // SYSTEM_H
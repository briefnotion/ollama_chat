#ifndef THOUGHTS_H
#define THOUGHTS_H

//#include <future>
//#include <iostream>

//#include "fled_time.h"

#include <initializer_list>

#include "stringthings.h"

#include "system.h"
#include "ollama_api.h"
#include "vectordb_python_api.h"

using namespace std;

// ------------------------------------------------------------------------- //

bool keyword_search(const string &Input, initializer_list<string> Words);
// Function to check for multiple words using an initializer list
// Example call: keyword_search("Hello world", {"hello", "world"});

// ------------------------------------------------------------------------- //

class THOUGHT
{
  public:

  // Variables for starting new process
  //  Answer all
  bool THINKING = false;        // Active Process
  string ABOUT = "";            // Active Process Name
  int THINKING_STAGE = -1;      // Stage of Process. -1 = off
  string SUBJECT = "";          // Criteria
};

class THOUGHTS
{
  private:

  vector<THOUGHT> TIDBITS;

  bool SYSTEM_MAITENANCE_MODE = false;

  bool INPUT_CHANGED = false;

  bool process_new_input_stages(SYSTEM &System, THOUGHT &Tidbit);

  public:

  // Ollama System
  OLLAMA_API      OLLAMA_SYSTEM;

  // ChromaDB System
  VECTORDB_PYTHON_API VECTORDB_SYSTEM;

  void input(string Input);

  void process_input(SYSTEM &System);
  void process_thinking(SYSTEM &System);
  void process(SYSTEM &System);
};

// ------------------------------------------------------------------------- //

#endif // THOUGHTS_H
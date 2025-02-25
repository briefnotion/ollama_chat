#ifndef THOUGHTS_H
#define THOUGHTS_H

//#include <future>
//#include <iostream>

//#include "fled_time.h"

#include "stringthings.h"

#include "system.h"
//#include "ollama_api.h"
#include "ollama_api_python.h"
#include "vectordb_python_api.h"
#include "remember.h"
#include "thought_variables.h"
#include "thought_stages.h"

using namespace std;

// ------------------------------------------------------------------------- //

bool keyword_search(const string &Input, initializer_list<string> Words);
// Function to check for multiple words using an initializer list
// Example call: keyword_search("Hello world", {"hello", "world"});

// ------------------------------------------------------------------------- //

class THOUGHTS
{
  private:

  // Stage Thought Routines
  THOUGHT_STAGES    STAGES;

  bool SYSTEM_MAITENANCE_MODE = false;

  void check_simple_keywords(THOUGHT &Thought);
  void check_intentions(THOUGHT &Thought);

  public:

  // Ollama System
  OLLAMA_API_PYTHON OLLAMA_SYSTEM;

  // Main variables
  THOUGHT_VARIABLES THOUGHT_CONTOL;

  private:
  void pop_latest_thought();

  void process_input(SYSTEM &System);
  void process_thinking(SYSTEM &System);
  void process_resolution();

  THOUGHTS_PROPERTIES PROPS;

  public:
  int thought_count();
  string thought_current();
  int thought_stage();
  bool isolated();

  void create();

  void interact_simple_ask(string Question, string Am_I_Asking_You_To);
  // Ask ollama to return yes no or maybe.
  // Results are placed in the RESOLUTION_BUFFER before resolving.
  // This is an internal command.
  
  void interact_input(string Input, bool Keyword_Search, string About);
  void interact_input(string Input);
  // Creates a new thought and adds it to the train of thoughts
  // Defaults:
  //  Keyword_Search = true
  //  About = "new input"

  void process(SYSTEM &System);

};

// ------------------------------------------------------------------------- //

#endif // THOUGHTS_H
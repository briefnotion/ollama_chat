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

  bool KEYWORD_SEARCH = true;

  bool RESOLOLUTION_FOUND = false;
  string RESOLOLUTION_FULL = "";
  int RESOLUTION_SIMPLE = 99; 
};

class THOUGHTS
{
  private:

  vector<THOUGHT> TRAIN_OF_THOUGH;

  bool SYSTEM_MAITENANCE_MODE = false;

  bool INPUT_CHANGED = false;

  void process_new_input_stages(SYSTEM &System, THOUGHT &Tidbit);

  public:
  // Ollama System
  OLLAMA_API      OLLAMA_SYSTEM;
  // ChromaDB System
  VECTORDB_PYTHON_API VECTORDB_SYSTEM;

  void input(string Input, bool Keyword_Search = true);

  private:
  bool thoughts_exist(vector<THOUGHT> &Train_of_Thoughts);
  THOUGHT& get_latest_thought(vector<THOUGHT> &Train_of_Thoughts);

  void process_input(SYSTEM &System, THOUGHT &Current_Thought);
  void process_thinking(SYSTEM &System, THOUGHT &Current_Thought);
  void process_resolution(vector<THOUGHT> &Train_of_Thoughts);

  public:
  void process(SYSTEM &System);
};

// ------------------------------------------------------------------------- //

#endif // THOUGHTS_H
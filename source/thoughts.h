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

class RESOLUTION_RESULTS
{
  public:

  bool CHANGED = false;         // Set to false after read to avoid re-reading if not changed

  bool RESOLOLUTION_FOUND = false;
  string RESOLOLUTION_FULL = "";
  int RESOLUTION_SIMPLE = 99;   // 99 Undefined
                                // -1 = Undeterimined
                                //  0 = false (NO)
                                //  1 = true  (YES)

  void clear();
};

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

  RESOLUTION_RESULTS RESOLUTION;

  //vector<THOUGHT> TRAIN_OF_THOUGH;
};

class THOUGHTS
{
  private:

  vector<THOUGHT> TRAIN_OF_THOUGH;

  bool SYSTEM_MAITENANCE_MODE = false;

  bool INPUT_CHANGED = false;

  RESOLUTION_RESULTS RESOLUTION_BUFFER;   // THIS IS PRECARIOUS. USE WITH CAUTION

  void process_input_stages(SYSTEM &System);
  void process_simple_ask_stages(SYSTEM &System);
  void process_maintenance_mode_stages(SYSTEM &System);
  void process_maintenance_mode_cycle(SYSTEM &System);

  public:
  // Ollama System
  OLLAMA_API      OLLAMA_SYSTEM;
  // ChromaDB System
  VECTORDB_PYTHON_API VECTORDB_SYSTEM;

  private:
  //bool thoughts_exist();
  //int int_thought_count();
  //THOUGHT& get_latest_thought(vector<THOUGHT> &Thought);
  void pop_latest_thought();

  void process_input(SYSTEM &System);
  void process_thinking(SYSTEM &System);
  void process_resolution();

  public:
  int thought_count();
  void simple_ask(string Question, string Am_I_Asking_You_To);
  void input(string Input, bool Keyword_Search = true, string About = "new input");
  void process(SYSTEM &System);
};

// ------------------------------------------------------------------------- //

#endif // THOUGHTS_H
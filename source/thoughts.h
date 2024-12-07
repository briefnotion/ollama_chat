#ifndef THOUGHTS_H
#define THOUGHTS_H

//#include <future>
//#include <iostream>

//#include "fled_time.h"

#include "stringthings.h"

#include "system.h"

#include "ollama_api.h"
#include "vectordb_python_api.h"

using namespace std;

// ------------------------------------------------------------------------- //

bool keyword_search(string Input);

// ------------------------------------------------------------------------- //

class THOUGHTS
{
  private:

  //string INPUT = "";

  bool SYSTEM_MAITENANCE_MODE = false;

  // Variables for starting new process
  //  Answer all
  bool THINKING = false;        // Active Process
  string ABOUT = "";            // Active Process Name
  int THINKING_STAGE = -1;      // Stage of Process. -1 = off
  string SUBJECT = "";          // Criteria

  bool CHANGED = false;

  void clear_thoughts();
  void process_new_input_stages(SYSTEM &System);

  public:

  // Ollama System
  OLLAMA_API      OLLAMA_SYSTEM;

  // ChromaDB System
  VECTORDB_PYTHON_API VECTORDB_SYSTEM;

  void input(string Input);

  //void process_input(SYSTEM &System);
  void process_thinking(SYSTEM &System);
  void process(SYSTEM &System);
};

// ------------------------------------------------------------------------- //

#endif // THOUGHTS_H
#ifndef THOUGHT_VARIABLES_H
#define THOUGHT_VARIABLES_H

#include <string>
#include <vector>

#include "stringthings.h"

#include "vectordb_python_api.h"
#include "remember.h"


using namespace std;

// ------------------------------------------------------------------------- //

class THOUGHTS_PROPERTIES
{
  public:

  string USER_NAME = "";  // placeholer

};

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
  bool ISOLATE_INPUT_TO_THOUGHT = false;
  bool JUMP_ISOLATE_INPUT_TO_THOUGHT = false;

  RESOLUTION_RESULTS RESOLUTION;
};

class THOUGHT_VARIABLES
{
  public:

  // ChromaDB System
  VECTORDB_PYTHON_API VECTORDB_SYSTEM;

  // Staged Thoughts
  vector<THOUGHT> TRAIN_OF_THOUGH;

  // Remember
  REMEMBER          MEMORY;

  // Resolution Buffer
  RESOLUTION_RESULTS RESOLUTION_BUFFER;   // THIS IS PRECARIOUS. USE WITH CAUTION

  bool INPUT_CHANGED = false;

  int thought_count();

};

// ------------------------------------------------------------------------- //


#endif // THOUGHT_VARIABLES_H
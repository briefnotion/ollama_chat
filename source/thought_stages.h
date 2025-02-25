#ifndef THOUGHT_STAGES_H
#define THOUGHT_STAGES_H

//#include <iostream>
#include "system.h"

#include "stringthings.h"

#include "ollama_api_python.h"
#include "thought_variables.h"

using namespace std;

// ------------------------------------------------------------------------- //

class THOUGHT_STAGES
{
  public:

  void interact_simple_ask(string Question, string Am_I_Asking_You_To, THOUGHT_VARIABLES &Thought_Control);
  // Ask ollama to return yes no or maybe.
  // Results are placed in the RESOLUTION_BUFFER before resolving.
  // This is an internal command.
  
  void interact_input(string Input, bool Keyword_Search, string About, THOUGHT_VARIABLES &Thought_Control);
  void interact_input(string Input, THOUGHT_VARIABLES &Thought_Control);
  // Creates a new thought and adds it to the train of thoughts
  // Defaults:
  //  Keyword_Search = true
  //  About = "new input"

  void process_input_stages(SYSTEM &System, THOUGHT_VARIABLES &Thought_Control,
                            THOUGHTS_PROPERTIES &Props, OLLAMA_API_PYTHON &Ollama_System);
  void process_simple_ask_stages(THOUGHT_VARIABLES &Thought_Control,
                            OLLAMA_API_PYTHON &Ollama_System);
  void process_maintenance_mode_stages(THOUGHT_VARIABLES &Thought_Control);
  void process_maintenance_mode_cycle(SYSTEM &System, THOUGHT_VARIABLES &Thought_Control,
                            OLLAMA_API_PYTHON &Ollama_System);
  void process_opening_mode_stages(SYSTEM &System, THOUGHT_VARIABLES &Thought_Control,
                            OLLAMA_API_PYTHON &Ollama_System);
  void process_in_conclusion_mode_stages(THOUGHT_VARIABLES &Thought_Control,
                            OLLAMA_API_PYTHON &Ollama_System);
};

// ------------------------------------------------------------------------- //


#endif // THOUGHTS_H
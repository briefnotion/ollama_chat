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
  private:

  int thinking_stage(THOUGHT_VARIABLES &Thought_Control);
  void thinking_stage_set(THOUGHT_VARIABLES &Thought_Control, int New_Stage);

  bool ready_to_think(OLLAMA_API_PYTHON &Ollama_System);
  bool ready_to_gather(OLLAMA_API_PYTHON &Ollama_System);
  bool ready_for_request(OLLAMA_API_PYTHON &Ollama_System);

  void cycle_open_and_intro(SYSTEM &System, THOUGHT_VARIABLES &Thought_Control,
                            OLLAMA_API_PYTHON &Ollama_System, 
                            string Cycle_Name, string Memory_File_Message);
    // Creates warning
    // Pauses context
    // Creates new stage
    // Shows Intro
    

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

  void process_memory_file_mode_stages(THOUGHT_VARIABLES &Thought_Control);
  void process_memory_file_mode_cycle(SYSTEM &System, THOUGHT_VARIABLES &Thought_Control,
                            OLLAMA_API_PYTHON &Ollama_System);
};

// ------------------------------------------------------------------------- //


#endif // THOUGHTS_H
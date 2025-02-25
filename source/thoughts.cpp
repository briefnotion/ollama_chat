#ifndef THOUGHTS_CPP
#define THOUGHTS_CPP

#include "thoughts.h"

using namespace std;

// ------------------------------------------------------------------------- //

void RESOLUTION_RESULTS::clear()
{
  CHANGED = false;         // Set to false after read to avoid re-reading if not changed

  RESOLOLUTION_FOUND = false;
  RESOLOLUTION_FULL = "";
  RESOLUTION_SIMPLE = 99;
}

void THOUGHTS::check_simple_keywords(THOUGHT &Thought)
{
  if (keyword_search(THOUGHT_CONTOL.TRAIN_OF_THOUGH.back().SUBJECT, {"time"}))
  {
    //Thought.SUBJECT += " (Update your internal clock to the current real time of " + current_time() + ".)";
    string dummy = Thought.SUBJECT;
    (void)dummy;
  }
  
  if (keyword_search(THOUGHT_CONTOL.TRAIN_OF_THOUGH.back().SUBJECT, {"date"}))
  {
    //Thought.SUBJECT += " (Update your internal clock to the current real date of " + current_date() + ".)";
    
  }
}

void THOUGHTS::check_intentions(THOUGHT &Thought)
{
  if (Thought.KEYWORD_SEARCH)
  {
    // First Check for keyword search. Run special conditions.
    if (keyword_search(THOUGHT_CONTOL.TRAIN_OF_THOUGH.back().SUBJECT, {"maintenance", "mode"}))
    {
      Thought.ABOUT = "maintenance mode";
    }
    else
    {
      Thought.ABOUT = "input";
    }
  }
}

void THOUGHTS::pop_latest_thought()
{
  THOUGHT_CONTOL.TRAIN_OF_THOUGH.pop_back();
}

void THOUGHTS::process_input(SYSTEM &System)
{
  // Accept and call and prepare the appropriate function based on the input
  
  if (THOUGHT_CONTOL.INPUT_CHANGED)
  {
    THOUGHT_CONTOL.INPUT_CHANGED = false;
    System.OUTPUT_INPUT.clear();

    if (THOUGHT_CONTOL.TRAIN_OF_THOUGH.back().ABOUT == "new input")
    {
      System.OUTPUT_OLLAMA_RESPONSE.add_to(" AI:\n     ", System.OUTPUT_FOCUS);

      check_simple_keywords(THOUGHT_CONTOL.TRAIN_OF_THOUGH.back());
      check_intentions(THOUGHT_CONTOL.TRAIN_OF_THOUGH.back());
    }
  }
}

void THOUGHTS::process_thinking(SYSTEM &System)
{
  THOUGHT_CONTOL.INPUT_CHANGED = false;
  
  // Route direction
  if (THOUGHT_CONTOL.TRAIN_OF_THOUGH.back().THINKING)
  {
    if (THOUGHT_CONTOL.TRAIN_OF_THOUGH.back().ABOUT == "input")
    {
      STAGES.process_input_stages(System, THOUGHT_CONTOL, PROPS, OLLAMA_SYSTEM);
    }
    else if (THOUGHT_CONTOL.TRAIN_OF_THOUGH.back().ABOUT == "simple ask")
    {
      STAGES.process_simple_ask_stages(THOUGHT_CONTOL, OLLAMA_SYSTEM);
    }
    else if (THOUGHT_CONTOL.TRAIN_OF_THOUGH.back().ABOUT == "maintenance mode")
    {
      STAGES.process_maintenance_mode_stages(THOUGHT_CONTOL);
    }
    else if (THOUGHT_CONTOL.TRAIN_OF_THOUGH.back().ABOUT == "maintenance mode cycle")
    {
      STAGES.process_maintenance_mode_cycle(System, THOUGHT_CONTOL, OLLAMA_SYSTEM);
    }
    else if (THOUGHT_CONTOL.TRAIN_OF_THOUGH.back().ABOUT == "opening")
    {
      STAGES.process_opening_mode_stages(System, THOUGHT_CONTOL, OLLAMA_SYSTEM);
    }
    else if (THOUGHT_CONTOL.TRAIN_OF_THOUGH.back().ABOUT == "in conclusion")
    {
      STAGES.process_in_conclusion_mode_stages(THOUGHT_CONTOL, OLLAMA_SYSTEM);
    }
  }
}

void THOUGHTS::process_resolution()
{
  if (THOUGHT_CONTOL.TRAIN_OF_THOUGH.back().RESOLUTION.RESOLOLUTION_FOUND == true)
  {
    THOUGHT_CONTOL.RESOLUTION_BUFFER = THOUGHT_CONTOL.TRAIN_OF_THOUGH.back().RESOLUTION;
    pop_latest_thought();
  }
}

int THOUGHTS::thought_count()
{
  return THOUGHT_CONTOL.thought_count();
}

string THOUGHTS::thought_current()
{
  if (thought_count() == 0)
  {
    return "idle";
  }
  else
  {
    return THOUGHT_CONTOL.TRAIN_OF_THOUGH.back().ABOUT;
  }
}

int THOUGHTS::thought_stage()
{
  if (thought_count() == 0)
  {
    return -1;
  }
  else
  {
    return THOUGHT_CONTOL.TRAIN_OF_THOUGH.back().THINKING_STAGE;
  }
}

bool THOUGHTS::isolated()
{
  if (thought_count() == 0)
  {
    return THOUGHT_CONTOL.TRAIN_OF_THOUGH.back().ISOLATE_INPUT_TO_THOUGHT;
  }
  else
  {
    return false;
  }
}

void THOUGHTS::create()
{
  OLLAMA_SYSTEM.OLLAMA_RESPONSE_THREAD.create(OLLAMA_RESPONSE_THREAD_TIMER_DELAY);
  THOUGHT_CONTOL.VECTORDB_SYSTEM.PYTHON_QUESTION_RESPONSE_THREAD.create(VECTORDB_API_RESPONSE_THREAD_TIMER_DELAY);
}

void THOUGHTS::interact_simple_ask(string Question, string Am_I_Asking_You_To)
{
  STAGES.interact_simple_ask(Question, Am_I_Asking_You_To, THOUGHT_CONTOL);
}

void THOUGHTS::interact_input(string Input, bool Keyword_Search, string About)
{
  STAGES.interact_input(Input, Keyword_Search, About, THOUGHT_CONTOL);
}

void THOUGHTS::interact_input(string Input)
{
  STAGES.interact_input(Input, THOUGHT_CONTOL);
}

void THOUGHTS::process(SYSTEM &System)
{
  if (THOUGHT_CONTOL.TRAIN_OF_THOUGH.size() > 0)
  {
    //THOUGHT& latest_thought = get_latest_thought(TRAIN_OF_THOUGH);

    // Check new input for special conditions.
    process_input(System);

    // Avoid current thought if another input has been made, to work on latest thought
    if (THOUGHT_CONTOL.INPUT_CHANGED == false)
    {
      // Thinking
      process_thinking(System);

      // Resolutions
      process_resolution();
    }
  }

  // Handle Output
  THOUGHT_CONTOL.VECTORDB_SYSTEM.process();
  OLLAMA_SYSTEM.process(System.OUTPUT_OLLAMA_RESPONSE, System.OUTPUT_FOCUS, THOUGHT_CONTOL.MEMORY);
}

// ------------------------------------------------------------------------- //

#endif //THOUGHTS_CPP
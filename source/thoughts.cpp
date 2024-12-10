#ifndef THOUGHTS_CPP
#define THOUGHTS_CPP

#include "thoughts.h"

using namespace std;

// Function to check for multiple words using an initializer list
bool keyword_search(const string &Input, initializer_list<string> Words) 
{
  for (const auto &word : Words) 
  {
    if (!string_contains_word(Input, word)) 
    {
      return false;
    }
  }
  return true;
}

// ------------------------------------------------------------------------- //

/*
void THOUGHT::clear_thoughts()
{
  THINKING = false;
  ABOUT = "";
  THINKING_STAGE = -1;      // Stage of Process. -1 = off
  SUBJECT = "";    
}
*/

// ------------------------------------------------------------------------- //

void THOUGHTS::process_new_input_stages(SYSTEM &System, THOUGHT &Tidbit)
{
  if (Tidbit.THINKING_STAGE == 0)
  {
    // submit request for docuentation

    VECTORDB_SYSTEM.search_db_for_relevant_docs(Tidbit.SUBJECT, "buildragwithpython");

    Tidbit.THINKING_STAGE = 1;
  }
  else if (Tidbit.THINKING_STAGE == 1)
  {
    if (VECTORDB_SYSTEM.get_status() == VECTORDB_API_RESPONSE_READY_TO_GATHER)
    {
      string gathered_documents = "";
      if (VECTORDB_SYSTEM.get_gathered_documents(gathered_documents))
      {
        System.OUTPUT_OLLAMA_RESPONSE.add_to("Documentation was found.\n", System.OUTPUT_FOCUS);  // Temporary Note for debugging
        string new_question = Tidbit.SUBJECT + " Use the following information, sourced from local files, provided: " + gathered_documents;
        OLLAMA_SYSTEM.submit_question(new_question);
      }
      else
      {
        OLLAMA_SYSTEM.submit_question(Tidbit.SUBJECT);
      }

      Tidbit.RESOLUTION_SIMPLE = -1;
      Tidbit.RESOLOLUTION_FULL = OLLAMA_SYSTEM.RESPONSE_FULL;
      Tidbit.RESOLOLUTION_FOUND = true;
    }
  }
}

void THOUGHTS::input(string Input, bool Keyword_Search)
{
  THOUGHT new_thought;

  INPUT_CHANGED = true;

  new_thought.THINKING = true;
  new_thought.ABOUT = "new input";
  new_thought.THINKING_STAGE = 0;
  new_thought.SUBJECT = Input;

  new_thought.KEYWORD_SEARCH = Keyword_Search;

  TRAIN_OF_THOUGH.push_back(new_thought);
}

bool THOUGHTS::thoughts_exist(vector<THOUGHT> &Train_of_Thoughts)
{
  if (Train_of_Thoughts.size() > 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

THOUGHT& THOUGHTS::get_latest_thought(vector<THOUGHT> &Train_of_Thoughts)
{
  return Train_of_Thoughts.back();
}

void THOUGHTS::process_input(SYSTEM &System, THOUGHT &Current_Thought)
{
  // Accept and call and prepare the appropriate function based on the input
  
  if (INPUT_CHANGED)
  {
    INPUT_CHANGED = false;

    if (Current_Thought.KEYWORD_SEARCH)
    {
      // First Check for keyword search. Run special conditions.
      if (keyword_search(Current_Thought.SUBJECT, {"maintenance", "mode"}))
      {
        // Testing
        //SYSTEM_MAITENANCE_MODE = true;

        string question = "Simply answer yes or no. If i say " + Current_Thought.SUBJECT + ". Am I asking you to enter the maintenance system?";

        System.OUTPUT_OLLAMA_RESPONSE.add_to(question, System.OUTPUT_FOCUS);
        System.OUTPUT_OLLAMA_RESPONSE.seperater(System.OUTPUT_FOCUS);

        input(question, false);
      }
      else
      {
        //System.OUTPUT_OLLAMA_RESPONSE.add_to("MAINTEANCEMODE set to false", System.OUTPUT_FOCUS);
        //System.OUTPUT_OLLAMA_RESPONSE.seperater(System.OUTPUT_FOCUS);
      }
    }

    System.OUTPUT_INPUT.clear();
  }

  /*
    // Check to see if in maintenance mode
    if (SYSTEM_MAITENANCE_MODE)
    {
      // Testing
      //if (INPUT.substr(0, 9) == " embeding")
      //{
      //  INPUT.erase(0, 9);
      //  System.VECTORDB_SYSTEM.submit_question(INPUT);
      //  System.OUTPUT_INPUT.clear();
      //}
      if (INPUT.substr(0, 7) == " import")
      {
        INPUT.erase(0, 7);
        System.VECTORDB_SYSTEM.submit_file_to_embed("buildragwithpython", INPUT);
        System.OUTPUT_INPUT.clear();
      }
      else if (INPUT.substr(0, 6) == " erase")
      {
        INPUT.erase(0, 6);
        System.VECTORDB_SYSTEM.submit_clear_database();
        System.OUTPUT_INPUT.clear();
      }
      else if (INPUT.substr(0, 5) == " list")
      {
        INPUT.erase(0, 5);
        System.VECTORDB_SYSTEM.submit_list_database();
        System.OUTPUT_INPUT.clear();
      }
      //else if (INPUT.substr(0, 2) == " i")
      //{
      //  INPUT.erase(0, 2);
      //  System.VECTORDB_SYSTEM.submit_question_to_ollama(INPUT, "i");
      //  System.OUTPUT_INPUT.clear();
      //}
      else if (INPUT.substr(0, 2) == " o")
      {
        INPUT.erase(0, 2);
        System.OLLAMA_SYSTEM.submit_question(INPUT);
        System.OUTPUT_INPUT.clear();
      } 
      //else if (INPUT.substr(0, 2) == " n")
      //{
      //  INPUT.erase(0, 2);
      //  System.VECTORDB_SYSTEM.submit_question_to_ollama_par(INPUT, "n", System.OLLAMA_SYSTEM);
      //  System.OUTPUT_INPUT.clear();
      //}
    }
    else
    {
      // First Check for keyword search
      if (keyword_search(INPUT))
      {
        // Testing
        SYSTEM_MAITENANCE_MODE = true;
        System.OUTPUT_OLLAMA_RESPONSE.add_to("MAINTEANCEMODE set to true", System.OUTPUT_FOCUS);
        System.OUTPUT_OLLAMA_RESPONSE.seperater(System.OUTPUT_FOCUS);
      }

      // submit request
      //System.VECTORDB_SYSTEM.submit_question_to_ollama_cos(INPUT, "buildragwithpython", "g", System.OLLAMA_SYSTEM);

    }

    // --- 
    CHANGED = false;
    //INPUT = "";
  }
  */
}

void THOUGHTS::process_thinking(SYSTEM &System, THOUGHT &Current_Thought)
{
  // Route direction
  if (Current_Thought.THINKING)
  {
    if (Current_Thought.ABOUT == "new input")
    {
      process_new_input_stages(System, Current_Thought);
    }
  }
}

void THOUGHTS::process_resolution(vector<THOUGHT> &Train_of_Thoughts)
{
  if (TRAIN_OF_THOUGH.back().RESOLOLUTION_FOUND)
  {
    TRAIN_OF_THOUGH.pop_back();
  }
}

void THOUGHTS::process(SYSTEM &System)
{
  if (thoughts_exist(TRAIN_OF_THOUGH))
  {
    //THOUGHT& latest_thought = get_latest_thought(TRAIN_OF_THOUGH);

    // Check new input for special conditions.
    process_input(System, get_latest_thought(TRAIN_OF_THOUGH));

    // Avoid current thought if another input has been made, to work on latest thought
    if (INPUT_CHANGED == false)
    {
      // Thinking
      process_thinking(System, get_latest_thought(TRAIN_OF_THOUGH));

      // Resolutions
      process_resolution(TRAIN_OF_THOUGH);
    }
  }

  // Handle Output
  VECTORDB_SYSTEM.process(System.OUTPUT_OLLAMA_RESPONSE, System.OUTPUT_FOCUS, OLLAMA_SYSTEM);
  OLLAMA_SYSTEM.process(System.OUTPUT_OLLAMA_RESPONSE, System.OUTPUT_FOCUS);
}

// ------------------------------------------------------------------------- //

#endif //THOUGHTS_CPP
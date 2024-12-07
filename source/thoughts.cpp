#ifndef THOUGHTS_CPP
#define THOUGHTS_CPP

#include "thoughts.h"

using namespace std;

bool keyword_search(string Input)
{
  if (string_contains_word(Input, "maintenance") && string_contains_word(Input, "mode"))
  {
    return true;
  }
  else
  {
    return false;
  }
}

// ------------------------------------------------------------------------- //

void THOUGHTS::clear_thoughts()
{
  THINKING = false;
  ABOUT = "";
  THINKING_STAGE = -1;      // Stage of Process. -1 = off
  SUBJECT = "";    
}

void THOUGHTS::process_new_input_stages(SYSTEM &System)
{
  if (THINKING_STAGE == 0)
  {
    // submit request for docuentation

    VECTORDB_SYSTEM.search_db_for_relevant_docs(SUBJECT, "buildragwithpython");

    THINKING_STAGE = 1;
  }
  else if (THINKING_STAGE == 1)
  {
    if (VECTORDB_SYSTEM.get_status() == VECTORDB_API_RESPONSE_READY_TO_GATHER)
    {
      string gathered_documents = "";
      if (VECTORDB_SYSTEM.get_gathered_documents(gathered_documents))
      {
        System.OUTPUT_OLLAMA_RESPONSE.add_to("Documentation was found.\n", System.OUTPUT_FOCUS);  // Temporary Note for debugging
        string new_question = SUBJECT + " Use the following information, sourced from local files, provided: " + gathered_documents;
        OLLAMA_SYSTEM.submit_question(new_question);
      }
      else
      {
        OLLAMA_SYSTEM.submit_question(SUBJECT);
      }

      clear_thoughts();

    }
  }
}

void THOUGHTS::input(string Input)
{
  //CHANGED = true;
  THINKING = true;
  ABOUT = "new input";
  THINKING_STAGE = 0;
  SUBJECT = Input;
}

/*
void THOUGHTS::process_input(SYSTEM &System)
{
  // Accept and call and prepare the appropriate function based on the input

  if (CHANGED)
  {
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
      /*
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

  
}
*/

void THOUGHTS::process_thinking(SYSTEM &System)
{
  // Route direction
  if (THINKING)
  {
    if (ABOUT == "new input")
    {
      process_new_input_stages(System);
      System.OUTPUT_INPUT.clear();
    }
  }
}

void THOUGHTS::process(SYSTEM &System)
{
  // Thinking
  process_thinking(System);

  // Handle Output
  VECTORDB_SYSTEM.process(System.OUTPUT_OLLAMA_RESPONSE, System.OUTPUT_FOCUS, OLLAMA_SYSTEM);
  OLLAMA_SYSTEM.process(System.OUTPUT_OLLAMA_RESPONSE, System.OUTPUT_FOCUS);
}

// ------------------------------------------------------------------------- //

#endif //THOUGHTS_CPP
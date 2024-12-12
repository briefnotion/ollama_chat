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

void RESOLUTION_RESULTS::clear()
{
  CHANGED = false;         // Set to false after read to avoid re-reading if not changed

  RESOLOLUTION_FOUND = false;
  RESOLOLUTION_FULL = "";
  RESOLUTION_SIMPLE = 99;
}

void THOUGHTS::process_input_stages(SYSTEM &System)
{
  if (TRAIN_OF_THOUGH.back().THINKING_STAGE == 0)
  {
    // submit request for docuentation

    VECTORDB_SYSTEM.search_db_for_relevant_docs(TRAIN_OF_THOUGH.back().SUBJECT, "buildragwithpython");

    TRAIN_OF_THOUGH.back().THINKING_STAGE = 1;
  }
  else if (TRAIN_OF_THOUGH.back().THINKING_STAGE == 1)
  {
    if (VECTORDB_SYSTEM.get_status() == VECTORDB_API_RESPONSE_READY_TO_GATHER)
    {
      string gathered_documents = "";
      if (VECTORDB_SYSTEM.get_gathered_documents(gathered_documents))
      {
        System.OUTPUT_OLLAMA_RESPONSE.add_to("   *---- Documentation was found.\n", System.OUTPUT_FOCUS);  // Temporary Note for debugging
        string new_question = TRAIN_OF_THOUGH.back().SUBJECT + " Use the following information, sourced from local files, provided: " + gathered_documents;
        OLLAMA_SYSTEM.submit_question(new_question);
      }
      else
      {
        OLLAMA_SYSTEM.submit_question(TRAIN_OF_THOUGH.back().SUBJECT);
      }

      TRAIN_OF_THOUGH.back().RESOLUTION.RESOLUTION_SIMPLE = 99;
      TRAIN_OF_THOUGH.back().RESOLUTION.RESOLOLUTION_FULL = OLLAMA_SYSTEM.RESPONSE_FULL;
      TRAIN_OF_THOUGH.back().RESOLUTION.RESOLOLUTION_FOUND = true;
    }
  }
}

void THOUGHTS::process_simple_ask_stages(SYSTEM &System)
{ 
  //System.OUTPUT_OLLAMA_RESPONSE.add_to("SA STAGE: " + to_string(TRAIN_OF_THOUGH.back().THINKING_STAGE) + "\n", System.OUTPUT_FOCUS);
  
  if (TRAIN_OF_THOUGH.back().THINKING_STAGE == 0)
  {
    TRAIN_OF_THOUGH.back().THINKING_STAGE = 1;
    //OLLAMA_SYSTEM.submit_question(TRAIN_OF_THOUGH.back().SUBJECT);
    OLLAMA_SYSTEM.submit_question_internally(TRAIN_OF_THOUGH.back().SUBJECT);
  }
  else if (TRAIN_OF_THOUGH.back().THINKING_STAGE == 1)
  {
    if (OLLAMA_SYSTEM.get_status() == OLLAMA_API_READY_FOR_REQUEST)
    {
      TRAIN_OF_THOUGH.back().RESOLUTION.RESOLOLUTION_FOUND = true;
      TRAIN_OF_THOUGH.back().RESOLUTION.RESOLOLUTION_FULL = OLLAMA_SYSTEM.get_complete_text_response();
      TRAIN_OF_THOUGH.back().RESOLUTION.CHANGED = true;

      if (string_contains_word(TRAIN_OF_THOUGH.back().RESOLUTION.RESOLOLUTION_FULL, "yes"))
      {
        TRAIN_OF_THOUGH.back().RESOLUTION.RESOLUTION_SIMPLE = 1;
      }
      else if (string_contains_word(TRAIN_OF_THOUGH.back().RESOLUTION.RESOLOLUTION_FULL, "no"))
      {
        TRAIN_OF_THOUGH.back().RESOLUTION.RESOLUTION_SIMPLE = 0;
      }
      else
      {
        TRAIN_OF_THOUGH.back().RESOLUTION.RESOLUTION_SIMPLE = -1;
      }
    } 
  }
}

void THOUGHTS::process_maintenance_mode_stages(SYSTEM &System)
{
  //System.OUTPUT_OLLAMA_RESPONSE.add_to("MM STAGE: " + to_string(TRAIN_OF_THOUGH.back().THINKING_STAGE) + "\n", System.OUTPUT_FOCUS);

  if (TRAIN_OF_THOUGH.back().THINKING_STAGE == 0)
  {
    TRAIN_OF_THOUGH.back().THINKING_STAGE = 1;
    simple_ask(TRAIN_OF_THOUGH.back().SUBJECT, "enter the maintenance system");
  }
  else if (TRAIN_OF_THOUGH.back().THINKING_STAGE == 1)
  {
    if (RESOLUTION_BUFFER.CHANGED)
    {
      if (RESOLUTION_BUFFER.RESOLUTION_SIMPLE == 1)
      {
        TRAIN_OF_THOUGH.back().ABOUT = "maintenance mode cycle";
        TRAIN_OF_THOUGH.back().THINKING_STAGE = 0;
      }
      else
      {
        TRAIN_OF_THOUGH.back().ABOUT = "input";
        TRAIN_OF_THOUGH.back().THINKING_STAGE = 0;
        
        //System.OUTPUT_OLLAMA_RESPONSE.add_to("Continue with question\n", System.OUTPUT_FOCUS);
      }
      
      RESOLUTION_BUFFER.clear();
    }
  }
}

void THOUGHTS::process_maintenance_mode_cycle(SYSTEM &System)
{
  if (TRAIN_OF_THOUGH.back().THINKING_STAGE == 0)
  {
    System.OUTPUT_OLLAMA_RESPONSE.add_to("   *---- Entering Maintetnance Cycle\n", System.OUTPUT_FOCUS);

    TRAIN_OF_THOUGH.back().THINKING_STAGE = 1;

    string introduction = "Say: You are now entering the maintenance system. Then provide the following list of commands related to maintaining a ChromaDB database and Retrieval Augmented Generation, also known as RAG support: embeding, import, erase, list";
    introduction += "Also, say that non of this is working yet";

    OLLAMA_SYSTEM.submit_question(introduction);
  }
  else
  {
    TRAIN_OF_THOUGH.back().RESOLUTION.RESOLOLUTION_FOUND = true;
  }
}

/*
bool THOUGHTS::thoughts_exist()
{
  if (TRAIN_OF_THOUGH.size() > 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}
*/

/*
int THOUGHTS::int_thought_count()
{
  int count = TRAIN_OF_THOUGH.size();

  if (count > 0)
  {
    for (int pos = 0; pos < count; pos++)
    {
      count += int_thought_count(Train_of_Thoughts[pos].TRAIN_OF_THOUGH);
    }
  }

  return count;
}
*/

/*
THOUGHT& THOUGHTS::get_latest_thought(vector<THOUGHT> &Thought)
{
  if (thoughts_exist(Thought.back().TRAIN_OF_THOUGH))
  {
    return get_latest_thought(Thought.back().TRAIN_OF_THOUGH);
  }
  else
  {
    return Thought.back();
  }
}
*/

void THOUGHTS::pop_latest_thought()
{
  TRAIN_OF_THOUGH.pop_back();
}

void THOUGHTS::process_input(SYSTEM &System)
{
  // Accept and call and prepare the appropriate function based on the input
  
  if (INPUT_CHANGED)
  {
    INPUT_CHANGED = false;

    if (TRAIN_OF_THOUGH.back().ABOUT == "new input")
    {
      if (TRAIN_OF_THOUGH.back().KEYWORD_SEARCH)
      {
        // First Check for keyword search. Run special conditions.
        if (keyword_search(TRAIN_OF_THOUGH.back().SUBJECT, {"maintenance", "mode"}))
        {
          TRAIN_OF_THOUGH.back().ABOUT = "maintenance mode";
        }
        else
        {
          TRAIN_OF_THOUGH.back().ABOUT = "input";
        }
      }

      System.OUTPUT_INPUT.clear();
    }
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

void THOUGHTS::process_thinking(SYSTEM &System)
{
  INPUT_CHANGED = false;
  
  // Route direction
  if (TRAIN_OF_THOUGH.back().THINKING)
  {
    if (TRAIN_OF_THOUGH.back().ABOUT == "input")
    {
      process_input_stages(System);
    }
    else if (TRAIN_OF_THOUGH.back().ABOUT == "simple ask")
    {
      process_simple_ask_stages(System);
    }
    else if (TRAIN_OF_THOUGH.back().ABOUT == "maintenance mode")
    {
      process_maintenance_mode_stages(System);
    }
    else if (TRAIN_OF_THOUGH.back().ABOUT == "maintenance mode cycle")
    {
      process_maintenance_mode_cycle(System);
    }
  }
}

void THOUGHTS::process_resolution()
{
  if (TRAIN_OF_THOUGH.back().RESOLUTION.RESOLOLUTION_FOUND == true)
  {
    RESOLUTION_BUFFER = TRAIN_OF_THOUGH.back().RESOLUTION;
    pop_latest_thought();
  }
}

int THOUGHTS::thought_count()
{
  return TRAIN_OF_THOUGH.size();
}

void THOUGHTS::simple_ask(string Question, string Am_I_Asking_You_To)
{
  THOUGHT new_thought;

  INPUT_CHANGED = true;

  string query = "Simply answer yes or no. If i say, " + Question + ", Am I requesting you to " + Am_I_Asking_You_To + "?";

  new_thought.THINKING = true;
  new_thought.ABOUT = "simple ask";
  new_thought.THINKING_STAGE = 0;
  new_thought.SUBJECT = query;
  
  new_thought.KEYWORD_SEARCH = false;

  TRAIN_OF_THOUGH.push_back(new_thought);
}

void THOUGHTS::input(string Input, bool Keyword_Search, string About)
{
  THOUGHT new_thought;

  INPUT_CHANGED = true;

  new_thought.THINKING = true;
  new_thought.ABOUT = About;
  new_thought.THINKING_STAGE = 0;
  new_thought.SUBJECT = Input;

  new_thought.KEYWORD_SEARCH = Keyword_Search;

  TRAIN_OF_THOUGH.push_back(new_thought);
}

void THOUGHTS::process(SYSTEM &System)
{
  if (TRAIN_OF_THOUGH.size() > 0)
  {
    //THOUGHT& latest_thought = get_latest_thought(TRAIN_OF_THOUGH);

    // Check new input for special conditions.
    process_input(System);

    // Avoid current thought if another input has been made, to work on latest thought
    if (INPUT_CHANGED == false)
    {
      // Thinking
      process_thinking(System);

      // Resolutions
      process_resolution();
    }
  }

  // Handle Output
  VECTORDB_SYSTEM.process(System.OUTPUT_OLLAMA_RESPONSE, System.OUTPUT_FOCUS, OLLAMA_SYSTEM);
  OLLAMA_SYSTEM.process(System.OUTPUT_OLLAMA_RESPONSE, System.OUTPUT_FOCUS);
}

// ------------------------------------------------------------------------- //

#endif //THOUGHTS_CPP
#ifndef THOUGHTS_CPP
#define THOUGHTS_CPP

#include "thoughts.h"

using namespace std;

// ------------------------------------------------------------------------- //
void THOUGHTS::input(string Input)
{
  INPUT = Input;
  CHANGED = true;
}

void THOUGHTS::process(SYSTEM &System)
{
  if (CHANGED)
  {
    // Testing
    if (INPUT.substr(0, 9) == " embeding")
    {
      INPUT.erase(0, 9);
      System.VECTORDB_SYSTEM.submit_question(INPUT);
      System.OUTPUT_INPUT.clear();
    }
    else if (INPUT.substr(0, 7) == " import")
    {
      INPUT.erase(0, 7);
      System.VECTORDB_SYSTEM.submit_file_to_embed(INPUT);
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
    else if (INPUT.substr(0, 2) == " i")
    {
      INPUT.erase(0, 2);
      System.VECTORDB_SYSTEM.submit_question_to_ollama(INPUT, "i");
      System.OUTPUT_INPUT.clear();
    }
    else if (INPUT.substr(0, 2) == " o")
    {
      INPUT.erase(0, 2);
      System.OLLAMA_SYSTEM.submit_question(INPUT);
      System.OUTPUT_INPUT.clear();
    }
    else if (INPUT.substr(0, 2) == " n")
    {
      INPUT.erase(0, 2);
      System.VECTORDB_SYSTEM.submit_question_to_ollama_par(INPUT, "n", System.OLLAMA_SYSTEM);
      System.OUTPUT_INPUT.clear();
    }
    else
    {
      // submit request
      System.VECTORDB_SYSTEM.submit_question_to_ollama_cos(INPUT, "g", System.OLLAMA_SYSTEM);
      System.OUTPUT_INPUT.clear();
    }

    // --- 
    CHANGED = false;
    INPUT = "";
  }
}

// ------------------------------------------------------------------------- //

#endif //THOUGHTS_CPP
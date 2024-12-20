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

    VECTORDB_SYSTEM.search_db_for_relevant_docs(TRAIN_OF_THOUGH.back().SUBJECT, VECTORDB_API_COLLECTION_DOCUMEMTATION);

    TRAIN_OF_THOUGH.back().THINKING_STAGE = 1;
  }
  else if (TRAIN_OF_THOUGH.back().THINKING_STAGE == 1)
  {
    if (VECTORDB_SYSTEM.get_status() == VECTORDB_API_RESPONSE_READY_TO_GATHER)
    {
      TRAIN_OF_THOUGH.back().THINKING_STAGE =  2;

      string gathered_documents = "";
      if (VECTORDB_SYSTEM.get_gathered_documents(gathered_documents))
      {
        System.OUTPUT_OLLAMA_RESPONSE.add_to("   *---- DOCUMENTATION WAS FOUND\n", System.OUTPUT_FOCUS);  // Temporary Note for debugging
        string new_question = TRAIN_OF_THOUGH.back().SUBJECT + " Use the following information, sourced from local files, provided: " + gathered_documents;
        OLLAMA_SYSTEM.submit_question(new_question);
      }
      else
      {
        OLLAMA_SYSTEM.submit_question(TRAIN_OF_THOUGH.back().SUBJECT);
      }
    }
  }
  else if (TRAIN_OF_THOUGH.back().THINKING_STAGE == 2)
  {
    if (OLLAMA_SYSTEM.get_status() == OLLAMA_API_READY_TO_GATHER)
    {
      OLLAMA_SYSTEM.set_status(OLLAMA_API_READY_FOR_REQUEST);

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
    if (OLLAMA_SYSTEM.get_status() == OLLAMA_API_READY_TO_GATHER)
    {
      OLLAMA_SYSTEM.set_status(OLLAMA_API_READY_FOR_REQUEST);

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
    interact_simple_ask(TRAIN_OF_THOUGH.back().SUBJECT, "enter the maintenance system");
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
  if (TRAIN_OF_THOUGH.back().THINKING_STAGE == 0) // Open the system and lock it in till it's exited
  {
    System.OUTPUT_OLLAMA_RESPONSE.add_to("   *---- ENTERING MAINTENANCE MODE CYCLE\n", System.OUTPUT_FOCUS);

    TRAIN_OF_THOUGH.back().THINKING_STAGE = 1;
    TRAIN_OF_THOUGH.back().ISOLATE_INPUT_TO_THOUGHT = true;
    TRAIN_OF_THOUGH.back().SUBJECT = "";

    //string introduction = "Say: You are now entering the maintenance system. Then provide the following list of commands related to maintaining a ChromaDB database and Retrieval Augmented Generation, also known as RAG support: embeding, import, erase, list";

    string introduction = R"(
Ollama, please list the ChromaDB commands for Retrieval Augmented Generation (RAG) support as follows:

1. **search <search terms or sentence>**
   - Searches the vector database for related documents.
   - **Example**: `search tell me about the quantumflux capacitor`

2. **embed <filename or directory>**
   - Imports all files, or a single file, into the vector database.
   - **Example**: `embed ~/testdocs`

3. **erase**
   - Erases all entries in the vector database.
   - **Example**: `erase`

4. **list**
   - Lists all entries in the vector database.
   - **Example**: `list`

Please make sure to keep it concise and in the specified format.
)";

    OLLAMA_SYSTEM.submit_question(introduction);
  }

  else if(TRAIN_OF_THOUGH.back().THINKING_STAGE == 1) // clear previous gather request
  {
    if (OLLAMA_SYSTEM.get_status() == OLLAMA_API_READY_TO_GATHER ||
        OLLAMA_SYSTEM.get_status() == OLLAMA_API_READY_FOR_REQUEST)
    {
      TRAIN_OF_THOUGH.back().THINKING_STAGE = 2;
      OLLAMA_SYSTEM.set_status(OLLAMA_API_READY_FOR_REQUEST);
    }
  }

  else if(TRAIN_OF_THOUGH.back().THINKING_STAGE == 2) // Find exit or process commands
  {
    if (OLLAMA_SYSTEM.get_status() == OLLAMA_API_READY_FOR_REQUEST)
    {
      if (TRAIN_OF_THOUGH.back().SUBJECT != "")
      {
        if (keyword_search(TRAIN_OF_THOUGH.back().SUBJECT, {"exit"}))
        {
          TRAIN_OF_THOUGH.back().THINKING_STAGE = 99;
          interact_simple_ask(TRAIN_OF_THOUGH.back().SUBJECT, "exit the maintenance mode");
        }
        else 
        {
          TRAIN_OF_THOUGH.back().THINKING_STAGE = 10;
        }
      }
    }
  }

  else if(TRAIN_OF_THOUGH.back().THINKING_STAGE == 10)  // Process Commands
  {
    if (TRAIN_OF_THOUGH.back().SUBJECT.substr(0, 6) == "search")    // Search Command
    {
      TRAIN_OF_THOUGH.back().THINKING_STAGE = 11;
      TRAIN_OF_THOUGH.back().SUBJECT.erase(0, 6);
      VECTORDB_SYSTEM.search_db_for_relevant_docs(TRAIN_OF_THOUGH.back().SUBJECT, VECTORDB_API_COLLECTION_DOCUMEMTATION);
    }

    else if (TRAIN_OF_THOUGH.back().SUBJECT.substr(0, 4) == "list") // List Command
    {
      TRAIN_OF_THOUGH.back().THINKING_STAGE = 11;
      TRAIN_OF_THOUGH.back().SUBJECT.erase(0, 4);
      VECTORDB_SYSTEM.submit_list_database();
    }

    else if (TRAIN_OF_THOUGH.back().SUBJECT.substr(0, 5) == "embed") // List Command
    {
      TRAIN_OF_THOUGH.back().THINKING_STAGE = 11;
      TRAIN_OF_THOUGH.back().SUBJECT.erase(0, 5);
      VECTORDB_SYSTEM.submit_file_to_embed(VECTORDB_API_COLLECTION_DOCUMEMTATION, TRAIN_OF_THOUGH.back().SUBJECT);
    }

    else if (TRAIN_OF_THOUGH.back().SUBJECT.substr(0, 5) == "erase") // List Command
    {
      TRAIN_OF_THOUGH.back().THINKING_STAGE = 11;
      TRAIN_OF_THOUGH.back().SUBJECT.erase(0, 5);
      VECTORDB_SYSTEM.submit_clear_database();
    }

    else                                                            // Unknown Command
    {
      System.OUTPUT_OLLAMA_RESPONSE.add_to("   *---- UNKNOWN COMMAND\n", System.OUTPUT_FOCUS);
      TRAIN_OF_THOUGH.back().SUBJECT = "";
      TRAIN_OF_THOUGH.back().THINKING_STAGE = 1;
    }

  }

  else if(TRAIN_OF_THOUGH.back().THINKING_STAGE == 11)  // Clear
  {
    TRAIN_OF_THOUGH.back().SUBJECT = "";
    TRAIN_OF_THOUGH.back().THINKING_STAGE = 12;
  }

  else if(TRAIN_OF_THOUGH.back().THINKING_STAGE == 12)  // Wait for Command Response
  {
    if (VECTORDB_SYSTEM.get_status() == VECTORDB_API_RESPONSE_READY_TO_GATHER)
    {
      TRAIN_OF_THOUGH.back().SUBJECT = "";
      TRAIN_OF_THOUGH.back().THINKING_STAGE = 1;

      string full_response = VECTORDB_SYSTEM.get_full_response();

      System.OUTPUT_OLLAMA_RESPONSE.add_to(full_response, System.OUTPUT_FOCUS);

      string summary = "Give a quick and short summary of: " + full_response;
      OLLAMA_SYSTEM.submit_question(summary);
    }

    else if (VECTORDB_SYSTEM.get_status() == VECTORDB_API_READY_FOR_REQUEST)
    {
      TRAIN_OF_THOUGH.back().SUBJECT = "";
      TRAIN_OF_THOUGH.back().THINKING_STAGE = 1;
      System.OUTPUT_OLLAMA_RESPONSE.add_to("something is wrong. nothing was processed.", System.OUTPUT_FOCUS);
    }

  }
  else if(TRAIN_OF_THOUGH.back().THINKING_STAGE == 99)  // Recieve answer for exiting Maintenance Mode
  {
    if (RESOLUTION_BUFFER.CHANGED)
    {
      if (RESOLUTION_BUFFER.RESOLUTION_SIMPLE == 1)     // If answered yes, exit cycle
      {
        System.OUTPUT_OLLAMA_RESPONSE.add_to("   *---- EXITING MAINTENANCE MODE CYCLE\n", System.OUTPUT_FOCUS);
        TRAIN_OF_THOUGH.back().THINKING_STAGE = 1000;
      }
      else                                              // if answered anything else, go to process command
      {
        TRAIN_OF_THOUGH.back().THINKING_STAGE = 10;
      }
      
      RESOLUTION_BUFFER.clear();
    }
  }
  else
  {
    if (OLLAMA_SYSTEM.get_status() == OLLAMA_API_READY_TO_GATHER)
    {
      OLLAMA_SYSTEM.set_status(OLLAMA_API_READY_FOR_REQUEST);
    }

    TRAIN_OF_THOUGH.back().RESOLUTION.RESOLOLUTION_FOUND = true;
  }
}

void THOUGHTS::process_in_conclusion_mode_stages(SYSTEM &System)
{
  if (TRAIN_OF_THOUGH.back().THINKING_STAGE == 0)
  {
    // submit request for docuentation

    //VECTORDB_SYSTEM.search_db_for_relevant_docs(TRAIN_OF_THOUGH.back().SUBJECT, VECTORDB_API_COLLECTION_DOCUMEMTATION);

    TRAIN_OF_THOUGH.back().THINKING_STAGE = 1;
  }
  else if (TRAIN_OF_THOUGH.back().THINKING_STAGE == 1)
  {
    TRAIN_OF_THOUGH.back().THINKING_STAGE = 2;
    OLLAMA_SYSTEM.submit_question(TRAIN_OF_THOUGH.back().SUBJECT);
  }
  else if (TRAIN_OF_THOUGH.back().THINKING_STAGE == 2)
  {
    if (OLLAMA_SYSTEM.get_status() == OLLAMA_API_READY_TO_GATHER)
    {
      OLLAMA_SYSTEM.set_status(OLLAMA_API_READY_FOR_REQUEST);
      
      MEMORY_FILE tmp_memory_file;

      tmp_memory_file.TITLE = "conversation_closing_previous";
      tmp_memory_file.CONTENT = OLLAMA_SYSTEM.get_complete_text_response();
      tmp_memory_file.IS_READY = true;

      MEMORY.FILE_MANAGER.add_file(tmp_memory_file);

      TRAIN_OF_THOUGH.back().RESOLUTION.RESOLOLUTION_FOUND = true;
    }
  }
}

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
    System.OUTPUT_INPUT.clear();

    if (TRAIN_OF_THOUGH.back().ABOUT == "new input")
    {
      System.OUTPUT_OLLAMA_RESPONSE.add_to(" AI:\n     ", System.OUTPUT_FOCUS);

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
    }
  }
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
    else if (TRAIN_OF_THOUGH.back().ABOUT == "in conclusion")
    {
      process_in_conclusion_mode_stages(System);
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

string THOUGHTS::thought_current()
{
  if (thought_count() == 0)
  {
    return "idle";
  }
  else
  {
    return TRAIN_OF_THOUGH.back().ABOUT;
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
    return TRAIN_OF_THOUGH.back().THINKING_STAGE;
  }
}

void THOUGHTS::interact_simple_ask(string Question, string Am_I_Asking_You_To)
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

void THOUGHTS::interact_input(string Input, bool Keyword_Search, string About)
{
  bool isolated = false;

  if (thought_count() > 0)
  {
    if (TRAIN_OF_THOUGH.back().ISOLATE_INPUT_TO_THOUGHT == true)
    {
      isolated = true;
    }
  }

  if (isolated)
  {
    INPUT_CHANGED = true;
    //TRAIN_OF_THOUGH.back().THINKING = true;
    TRAIN_OF_THOUGH.back().SUBJECT = Input;
  }
  else
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
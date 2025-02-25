#ifndef THOUGHT_STAGES_CPP
#define THOUGHT_STAGES_CPP

#include "thought_stages.h"

using namespace std;

// ------------------------------------------------------------------------- //

void THOUGHT_STAGES::interact_simple_ask(string Question, string Am_I_Asking_You_To, THOUGHT_VARIABLES &Thought_Control)
{
  THOUGHT new_thought;

  Thought_Control.INPUT_CHANGED = true;

  string query = "Simply answer yes or no. If i say, " + Question + ", Am I requesting you to " + Am_I_Asking_You_To + "?";

  new_thought.THINKING = true;
  new_thought.ABOUT = "simple ask";
  new_thought.THINKING_STAGE = 0;
  new_thought.SUBJECT = query;
  
  new_thought.KEYWORD_SEARCH = false;

  Thought_Control.TRAIN_OF_THOUGH.push_back(new_thought);
}

void THOUGHT_STAGES::interact_input(string Input, bool Keyword_Search, string About, THOUGHT_VARIABLES &Thought_Control)
{
  bool isolated = false;

  // Process as new thought if left blank.
  if (About == "")
  {
    About = "new input";
  }

  // Stay Isollated if ISOLATE_INPUT_TO_THOUGHT, or disregard.
  if (Thought_Control.thought_count() > 0)
  {
    if (Thought_Control.TRAIN_OF_THOUGH.back().JUMP_ISOLATE_INPUT_TO_THOUGHT == false)
    {
      if (Thought_Control.TRAIN_OF_THOUGH.back().ISOLATE_INPUT_TO_THOUGHT == true)
      {
        isolated = true;
      }
    }
  }

  // Only modify the current thought if isolated
  if (isolated)
  {
    Thought_Control.INPUT_CHANGED = true;
    Thought_Control.TRAIN_OF_THOUGH.back().SUBJECT = Input;
  }
  else
  {
    // Create a new thought.
    THOUGHT new_thought;

    Thought_Control.INPUT_CHANGED = true;

    new_thought.THINKING = true;
    new_thought.ABOUT = About;
    new_thought.THINKING_STAGE = 0;
    new_thought.SUBJECT = Input;

    new_thought.KEYWORD_SEARCH = Keyword_Search;

    Thought_Control.TRAIN_OF_THOUGH.push_back(new_thought);
  }
}

void THOUGHT_STAGES::interact_input(string Input, THOUGHT_VARIABLES &Thought_Control)
{
  interact_input(Input, true, "", Thought_Control);
}

void THOUGHT_STAGES::process_input_stages(SYSTEM &System, THOUGHT_VARIABLES &Thought_Control,
                                          THOUGHTS_PROPERTIES &Props, OLLAMA_API_PYTHON &Ollama_System)
{
  if (Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE == 0)
  {
    // submit request for docuentation

    Thought_Control.VECTORDB_SYSTEM.search_db_for_relevant_docs(Thought_Control.TRAIN_OF_THOUGH.back().SUBJECT, VECTORDB_API_COLLECTION_DOCUMEMTATION);

    Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE = 1;
  }
  else if (Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE == 1)
  {
    if (Thought_Control.VECTORDB_SYSTEM.get_status() == VECTORDB_API_RESPONSE_READY_TO_GATHER)
    {
      Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE =  2;

      string gathered_documents = "";
      if (Thought_Control.VECTORDB_SYSTEM.get_gathered_documents(gathered_documents))
      {
        System.OUTPUT_OLLAMA_RESPONSE.add_to("   *---- DOCUMENTATION WAS FOUND\n", System.OUTPUT_FOCUS);  // Temporary Note for debugging
        string assisted_documents = " Use the following information, sourced from local files, provided: " + gathered_documents;
        Ollama_System.submit_request(ROLE_USER, "Documenation", assisted_documents, ROLE_USER, Props.USER_NAME, Thought_Control.TRAIN_OF_THOUGH.back().SUBJECT, true, true, true, false);
      }
      else
      {
        Ollama_System.submit_request(ROLE_USER, Props.USER_NAME, Thought_Control.TRAIN_OF_THOUGH.back().SUBJECT, true, true, true, true);
      }
    }
  }
  else if (Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE == 2)
  {
    if (Ollama_System.get_status() == OLLAMA_API_READY_TO_GATHER || 
    Ollama_System.get_status() == OLLAMA_API_READY_FOR_REQUEST)
    {
      Ollama_System.set_status(OLLAMA_API_READY_FOR_REQUEST);

      Thought_Control.TRAIN_OF_THOUGH.back().RESOLUTION.RESOLUTION_SIMPLE = 99;
      Thought_Control.TRAIN_OF_THOUGH.back().RESOLUTION.RESOLOLUTION_FULL = Ollama_System.RESPONSE_FULL;
      Thought_Control.TRAIN_OF_THOUGH.back().RESOLUTION.RESOLOLUTION_FOUND = true;
    }
  }
}

void THOUGHT_STAGES::process_simple_ask_stages(THOUGHT_VARIABLES &Thought_Control,
                                                OLLAMA_API_PYTHON &Ollama_System)
{ 
  if (Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE == 0)
  {
    Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE = 1;
    Ollama_System.context_pause();
    Ollama_System.submit_request(ROLE_USER, USER_AI, Thought_Control.TRAIN_OF_THOUGH.back().SUBJECT, false, false, false, false);
  }
  else if (Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE == 1)
  {
    if (Ollama_System.get_status() == OLLAMA_API_READY_TO_GATHER)
    {
      Ollama_System.set_status(OLLAMA_API_READY_FOR_REQUEST);

      Thought_Control.TRAIN_OF_THOUGH.back().RESOLUTION.RESOLOLUTION_FOUND = true;
      Thought_Control.TRAIN_OF_THOUGH.back().RESOLUTION.RESOLOLUTION_FULL = Ollama_System.get_complete_text_response();
      Thought_Control.TRAIN_OF_THOUGH.back().RESOLUTION.CHANGED = true;

      if (string_contains_word(Thought_Control.TRAIN_OF_THOUGH.back().RESOLUTION.RESOLOLUTION_FULL, "yes"))
      {
        Thought_Control.TRAIN_OF_THOUGH.back().RESOLUTION.RESOLUTION_SIMPLE = 1;
      }
      else if (string_contains_word(Thought_Control.TRAIN_OF_THOUGH.back().RESOLUTION.RESOLOLUTION_FULL, "no"))
      {
        Thought_Control.TRAIN_OF_THOUGH.back().RESOLUTION.RESOLUTION_SIMPLE = 0;
      }
      else
      {
        Thought_Control.TRAIN_OF_THOUGH.back().RESOLUTION.RESOLUTION_SIMPLE = -1;
      }

      Ollama_System.context_unpause();
    } 
  }
}

void THOUGHT_STAGES::process_maintenance_mode_stages(THOUGHT_VARIABLES &Thought_Control)
{
  //System.OUTPUT_OLLAMA_RESPONSE.add_to("MM STAGE: " + to_string(TRAIN_OF_THOUGH.back().THINKING_STAGE) + "\n", System.OUTPUT_FOCUS);

  if (Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE == 0)
  {
    Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE = 1;
    interact_simple_ask(Thought_Control.TRAIN_OF_THOUGH.back().SUBJECT, "enter the maintenance system", Thought_Control);
  }
  else if (Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE == 1)
  {
    if (Thought_Control.RESOLUTION_BUFFER.CHANGED)
    {
      if (Thought_Control.RESOLUTION_BUFFER.RESOLUTION_SIMPLE == 1)
      {
        Thought_Control.TRAIN_OF_THOUGH.back().ABOUT = "maintenance mode cycle";
        Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE = 0;
      }
      else
      {
        Thought_Control.TRAIN_OF_THOUGH.back().ABOUT = "input";
        Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE = 0;
        
        //System.OUTPUT_OLLAMA_RESPONSE.add_to("Continue with question\n", System.OUTPUT_FOCUS);
      }
      
      Thought_Control.RESOLUTION_BUFFER.clear();
    }
  }
}

void THOUGHT_STAGES::process_maintenance_mode_cycle(SYSTEM &System, THOUGHT_VARIABLES &Thought_Control,
                                                    OLLAMA_API_PYTHON &Ollama_System)
{
  if (Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE == 0) // Open the system and lock it in till it's exited
  {
    System.OUTPUT_OLLAMA_RESPONSE.add_to("   *---- ENTERING MAINTENANCE MODE CYCLE\n", System.OUTPUT_FOCUS);
    System.OUTPUT_OLLAMA_RESPONSE.add_to("   *---- THE FOLLOWING CONVERSATION WILL BE FORGOTTEN ON EXIT\n", System.OUTPUT_FOCUS);

    Ollama_System.context_pause();
    Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE = 1;
    Thought_Control.TRAIN_OF_THOUGH.back().ISOLATE_INPUT_TO_THOUGHT = true;
    Thought_Control.TRAIN_OF_THOUGH.back().SUBJECT = "";

    string introduction = Thought_Control.MEMORY.FILE_MANAGER.get_file("maintenance_mode_introduction");

    // Turn on and off tools.
    Ollama_System.TOOLS.WEATHER_TOOL.enable_set(false);
    Ollama_System.TOOLS.CLOCK_TOOL.enable_set(false);
    Ollama_System.TOOLS.DATE_TOOL.enable_set(false);
    Ollama_System.TOOLS.SYSTEM_HELP.enable_set(false);
    Ollama_System.TOOLS.MEMORY_FILES_LIST.enable_set(true);
    Ollama_System.TOOLS.MEMORY_FILES_PRINT.enable_set(true);

    Ollama_System.submit_request(ROLE_USER, USER_INSTRUCTIONS, introduction, true, true, true, false);
  }

  else if(Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE == 1) // clear previous gather request
  {
    if (Ollama_System.get_status() == OLLAMA_API_READY_TO_GATHER ||
        Ollama_System.get_status() == OLLAMA_API_READY_FOR_REQUEST)
    {
      Thought_Control.TRAIN_OF_THOUGH.back().JUMP_ISOLATE_INPUT_TO_THOUGHT = false;
      Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE = 2;
      Ollama_System.set_status(OLLAMA_API_READY_FOR_REQUEST);
    }
  }

  else if(Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE == 2) // Find exit or process commands
  {
    if (Ollama_System.get_status() == OLLAMA_API_READY_FOR_REQUEST)
    {
      if (Thought_Control.TRAIN_OF_THOUGH.back().SUBJECT != "")
      {
        if (keyword_search(Thought_Control.TRAIN_OF_THOUGH.back().SUBJECT, {"exit"}))
        {
          Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE = 99;
          interact_simple_ask(Thought_Control.TRAIN_OF_THOUGH.back().SUBJECT, "exit the maintenance mode", Thought_Control);
        }
        else 
        {
          Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE = 10;
        }
      }
    }
  }

  else if(Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE == 10)  // Process Commands
  {
    if (Thought_Control.TRAIN_OF_THOUGH.back().SUBJECT.substr(0, 6) == "search")    // Search Command
    {
      Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE = 11;
      Thought_Control.TRAIN_OF_THOUGH.back().SUBJECT.erase(0, 6);
      Thought_Control.VECTORDB_SYSTEM.search_db_for_relevant_docs(Thought_Control.TRAIN_OF_THOUGH.back().SUBJECT, VECTORDB_API_COLLECTION_DOCUMEMTATION);
    }

    else if (Thought_Control.TRAIN_OF_THOUGH.back().SUBJECT.substr(0, 4) == "list") // List Command
    {
      Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE = 11;
      Thought_Control.TRAIN_OF_THOUGH.back().SUBJECT.erase(0, 4);
      Thought_Control.VECTORDB_SYSTEM.submit_list_database();
    }

    else if (Thought_Control.TRAIN_OF_THOUGH.back().SUBJECT.substr(0, 5) == "embed") // List Command
    {
      Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE = 11;
      Thought_Control.TRAIN_OF_THOUGH.back().SUBJECT.erase(0, 5);
      Thought_Control.VECTORDB_SYSTEM.submit_file_to_embed(VECTORDB_API_COLLECTION_DOCUMEMTATION, Thought_Control.TRAIN_OF_THOUGH.back().SUBJECT);
    }

    else if (Thought_Control.TRAIN_OF_THOUGH.back().SUBJECT.substr(0, 5) == "erase") // List Command
    {
      Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE = 11;
      Thought_Control.TRAIN_OF_THOUGH.back().SUBJECT.erase(0, 5);
      Thought_Control.VECTORDB_SYSTEM.submit_clear_database();
    }

    else                                                            // Unknown Command
    {
      string new_question = Thought_Control.TRAIN_OF_THOUGH.back().SUBJECT;
      
      Thought_Control.TRAIN_OF_THOUGH.back().SUBJECT = "";
      Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE = 1;
      Thought_Control.TRAIN_OF_THOUGH.back().JUMP_ISOLATE_INPUT_TO_THOUGHT = true;

      interact_input(new_question, Thought_Control);
    }

  }

  else if(Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE == 11)  // Clear
  {
    Thought_Control.TRAIN_OF_THOUGH.back().SUBJECT = "";
    Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE = 12;
  }

  else if(Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE == 12)  // Wait for Command Response
  {
    if (Thought_Control.VECTORDB_SYSTEM.get_status() == VECTORDB_API_RESPONSE_READY_TO_GATHER)
    {
      Thought_Control.TRAIN_OF_THOUGH.back().SUBJECT = "";
      Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE = 1;

      string full_response = Thought_Control.VECTORDB_SYSTEM.get_full_response();

      //System.OUTPUT_OLLAMA_RESPONSE.add_to(full_response, System.OUTPUT_FOCUS);

      string summary = "Give a quick and short summary of: " + full_response;
      Ollama_System.submit_request(ROLE_USER, USER_AI, summary, true, false, true, false);
    }

    else if (Thought_Control.VECTORDB_SYSTEM.get_status() == VECTORDB_API_READY_FOR_REQUEST)
    {
      Thought_Control.TRAIN_OF_THOUGH.back().SUBJECT = "";
      Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE = 1;
      System.OUTPUT_OLLAMA_RESPONSE.add_to("something is wrong. nothing was processed.", System.OUTPUT_FOCUS);
    }

  }
  else if(Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE == 99)  // Recieve answer for exiting Maintenance Mode
  {
    if (Thought_Control.RESOLUTION_BUFFER.CHANGED)
    {
      if (Thought_Control.RESOLUTION_BUFFER.RESOLUTION_SIMPLE == 1)     // If answered yes, exit cycle
      {
        Ollama_System.context_unpause();
        System.OUTPUT_OLLAMA_RESPONSE.add_to("   *---- EXITING MAINTENANCE MODE CYCLE\n", System.OUTPUT_FOCUS);
        Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE = 1000;

        // Turn on and off tools.
        Ollama_System.TOOLS.WEATHER_TOOL.enable_set(true);
        Ollama_System.TOOLS.CLOCK_TOOL.enable_set(true);
        Ollama_System.TOOLS.DATE_TOOL.enable_set(true);
        Ollama_System.TOOLS.SYSTEM_HELP.enable_set(true);
        Ollama_System.TOOLS.MEMORY_FILES_LIST.enable_set(true);
        Ollama_System.TOOLS.MEMORY_FILES_PRINT.enable_set(false);

      }
      else                                              // if answered anything else, go to process command
      {
        Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE = 10;
      }
      
      Thought_Control.RESOLUTION_BUFFER.clear();
    }
  }
  else
  {
    if (Ollama_System.get_status() == OLLAMA_API_READY_TO_GATHER)
    {
      Ollama_System.set_status(OLLAMA_API_READY_FOR_REQUEST);
    }

    Thought_Control.TRAIN_OF_THOUGH.back().RESOLUTION.RESOLOLUTION_FOUND = true;
  }
}

void THOUGHT_STAGES::process_opening_mode_stages(SYSTEM &System, THOUGHT_VARIABLES &Thought_Control,
                                                  OLLAMA_API_PYTHON &Ollama_System)
{
  if (Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE == 0)
  {
    Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE = 1;
  }
  else if (Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE == 1)
  {
    Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE = 2;

    //string system_set = "You are a helpful assistant.";
    //string system_set = "You find the most appropriate tool or tools to execute, along with the parameters required to run the tool. Respond as JSON using the following schema: {\"functionName\": \"function name\", \"parameters\": [{\"parameterName\": \"name of parameter\", \"parameterValue\": \"value of parameter\"}]}. The tools are: " +  Ollama_System.tools();
    //string system_set = "If needed, you find the most appropriate tool or tools to execute, along with the parameters required to run the tool. The tools are: " +  Ollama_System.tools();

    string opening_intro = 
      //"Continue or current conversation from the following summary, and disregard any mentions of, not having a previous conversation: ";
      //"Remember these things, but disregard any mentions of, not having a previous conversation: ";
      //"Continue or current conversation from the following summary: ";
      "";

    System.OUTPUT_OLLAMA_RESPONSE.add_to(Thought_Control.MEMORY.load_memory_files(), System.OUTPUT_FOCUS);
    System.OUTPUT_OLLAMA_RESPONSE.seperater(System.OUTPUT_FOCUS);
    
    string opening_full = opening_intro + Thought_Control.MEMORY.FILE_MANAGER.get_file("conversation_closing_previous");

    System.OUTPUT_OLLAMA_RESPONSE.add_to(opening_full, System.OUTPUT_FOCUS);

    //Ollama_System.submit_request(ROLE_SYSTEM, "", system_set,
    //                              ROLE_USER, USER_AI, opening_full,
    //                              true, true, true, false);

    Ollama_System.submit_request(ROLE_ASSISTANT, USER_AI, opening_full,
                                  true, true, true, false);

  }
  else if (Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE == 2)
  {
    if (Ollama_System.get_status() == OLLAMA_API_READY_TO_GATHER)
    {
      Ollama_System.set_status(OLLAMA_API_READY_FOR_REQUEST);
      Thought_Control.TRAIN_OF_THOUGH.back().RESOLUTION.RESOLOLUTION_FOUND = true;
    }
  }
}

void THOUGHT_STAGES::process_in_conclusion_mode_stages(THOUGHT_VARIABLES &Thought_Control,
                                                        OLLAMA_API_PYTHON &Ollama_System)
{
  if (Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE == 0)
  {
    Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE = 1;
  }
  else if (Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE == 1)
  {
    Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE = 2;
    Ollama_System.submit_request(ROLE_USER, USER_AI, Thought_Control.TRAIN_OF_THOUGH.back().SUBJECT, false, true, true, false);
  }
  else if (Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE == 2)
  {
    if (Ollama_System.get_status() == OLLAMA_API_READY_TO_GATHER)
    {
      Ollama_System.set_status(OLLAMA_API_READY_FOR_REQUEST);
      
      MEMORY_FILE tmp_memory_file;

      tmp_memory_file.TITLE = "conversation_closing_previous";
      tmp_memory_file.CONTENT = Ollama_System.get_complete_text_response();
      tmp_memory_file.IS_READY = true;

      Thought_Control.MEMORY.FILE_MANAGER.add_file(tmp_memory_file);

      Thought_Control.TRAIN_OF_THOUGH.back().RESOLUTION.RESOLOLUTION_FOUND = true;
    }
  }
}

// ------------------------------------------------------------------------- //


#endif //THOUGHTS_STAGES_CPP
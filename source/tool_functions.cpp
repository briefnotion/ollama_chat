#ifndef TOOL_FUNCTIONS_CPP
#define TOOL_FUNCTIONS_CPP

#include "tool_functions.h"

using namespace std;

// ------------------------------------------------------------------------- //

bool TOOL_TRACKER::enabled()
{
  return ENABLED;
}

void TOOL_TRACKER::enable_set(bool Enable)
{
  ENABLED = Enable;
}

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //

nlohmann::json TOOL_FUNCTIONS::build_new_tool_reply_start(nlohmann::json Message)
{
  // ************* Include ID or INDEX
  /*
  {
  "tool_calls": {
    "function": {
      "arguments": {},
      "index": 2,
      "name": "get_current_date"
    }
  }
  */
 
  nlohmann::json tmpjson;
  tmpjson["content"] = nullptr;
  tmpjson["role"] = "assistant";
  tmpjson["tool_calls"] = {Message["tool_calls"]};

  return tmpjson;
}

// ------------------------------------------------------------------------- //
// Weather from testing,  always responds the same.
nlohmann::json TOOL_FUNCTIONS::weather_tool()
{
  return 
  {
    {"type", "function"},
    {"function", {
      {"name", "get_current_weather"},
      {"description", "Get the current weather for a location"},
      {"parameters", {
        {"type", "object"},
        {"properties", {
          {"location", {
            {"type", "string"},
            {"description", "The location to get the weather for, e.g. San Francisco, CA"}
          }},
          {"format", {
            {"type", "string"},
            {"description", "The format to return the weather in, e.g. 'celsius' or 'fahrenheit'"},
            {"enum", {"celsius", "fahrenheit"}}
          }}
        }},
        {"required", {"location", "format"}}
      }}
    }}
  };

}

nlohmann::json TOOL_FUNCTIONS::weather_tool_reply()
{
  if (WEATHER_TOOL_PARAM_FORMAT == "celsius")
  {
    return
    {
      {"role", "tool"},
      {"content", "The current weather in " + WEATHER_TOOL_PARAM_LOCATION + " has a temperature of 25° " + WEATHER_TOOL_PARAM_FORMAT + "."}, 
      {"name", "get_current_weather"}
    };
  }
  else if (WEATHER_TOOL_PARAM_FORMAT == "fahrenheit")
  {
    return
    {
      {"role", "tool"},
      {"content", "The current weather in " + WEATHER_TOOL_PARAM_LOCATION + " has a temperature of 77° " + WEATHER_TOOL_PARAM_FORMAT + "."}, 
      {"name", "get_current_weather"}
    };
  }
  else
  {
    return
    {
      {"role", "tool"},
      {"content", "I need fahrenheit or celsius?"}, 
      {"name", "get_current_weather"}
    };
  }
}

bool TOOL_FUNCTIONS::WEATHER_TOOL_CALL(nlohmann::json Message, nlohmann::json &Tool_Reply)
{
  bool ret_submitted = false;

  // get current weather
  if (Message["tool_calls"]["function"]["name"] == "get_current_weather")
  {
    ret_submitted = true;
    if(WEATHER_TOOL.enabled())
    {

      if (Message["tool_calls"]["function"].contains("arguments"))
      {
        if (Message["tool_calls"]["function"]["arguments"].contains("format") &&
        Message["tool_calls"]["function"]["arguments"].contains("location"))
        {
          WEATHER_TOOL_PARAM_FORMAT = Message["tool_calls"]["function"]["arguments"]["format"];
          WEATHER_TOOL_PARAM_LOCATION = Message["tool_calls"]["function"]["arguments"]["location"];
          
          Tool_Reply.push_back(build_new_tool_reply_start(Message));
          Tool_Reply.push_back(weather_tool_reply());
        }
      }
    }
  }

  return ret_submitted;
}

// ------------------------------------------------------------------------- //
// Date and TIme

nlohmann::json TOOL_FUNCTIONS::clock_tool()
{
  return 
  {
    {"type", "function"},
    {"function", {
      {"name", "get_current_time"},
      {"description", "The user is asking for the current time"},
      //{"parameters", {}}
      {"parameters", {
        {"type", "object"},
        {"properties", {
          {"format", {
            {"type", "string"},
            {"description", "The format to return the weather in, e.g. '24-Hour', '12-Hour' or 'Military'"},
            {"enum", {"24-Hour", "12-Hour", "Military"}}
          }}
        }},
        {"required", {"location", "format"}}
      }}
    }}
  };
}

nlohmann::json TOOL_FUNCTIONS::clock_tool_reply()
{
  if (CLOCK_TOOL_PARAM_FORMAT == "24-Hour" or CLOCK_TOOL_PARAM_FORMAT == "Military")
  {
    return
    {
      {"role", "tool"},
      {"content", "The time is " + current_time(false) + "."}, 
      {"name", "get_current_time"}
    };
  }
  else
  {    return
    {
      {"role", "tool"},
      {"content", "The time is " + current_time(true) + "."}, 
      {"name", "get_current_time"}
    };
  }
}

bool TOOL_FUNCTIONS::CLOCK_TOOL_CALL(nlohmann::json Message, nlohmann::json &Tool_Reply)
{
  bool ret_submitted = false;

  if (Message["tool_calls"]["function"]["name"] == "get_current_time")
  {
    ret_submitted = true;
    if(CLOCK_TOOL.enabled())
    {
      if (Message["tool_calls"]["function"].contains("arguments"))
      {
        if (Message["tool_calls"]["function"]["arguments"].contains("format"))
        {
          CLOCK_TOOL_PARAM_FORMAT = Message["tool_calls"]["function"]["arguments"]["format"];
          
          Tool_Reply.push_back(build_new_tool_reply_start(Message));
          Tool_Reply.push_back(clock_tool_reply());
          dump_string(DUMP_DIRECTORY, "test.json", Tool_Reply.dump(2));
        }
      }
    }
  }

  return ret_submitted;
}

nlohmann::json TOOL_FUNCTIONS::date_tool()
{
  return 
  {
    {"type", "function"},
    {"function", {
      {"name", "get_current_date"},
      {"description", "The user is asking for the current date"}
      //{"parameters", {}}
    }}
  };
}

nlohmann::json TOOL_FUNCTIONS::date_tool_reply()
{
  return
  {
    {"role", "tool"},
    {"content", "The date is " + current_date() + "."}, 
    {"name", "get_current_date"}
  };
}

bool TOOL_FUNCTIONS::DATE_TOOL_CALL(nlohmann::json Message, nlohmann::json &Tool_Reply)
{
  bool ret_submitted = false;

    if (Message["tool_calls"]["function"]["name"] == "get_current_date")
    {
      ret_submitted = true;
      if(DATE_TOOL.enabled())
      {
        Tool_Reply.push_back(build_new_tool_reply_start(Message));
        Tool_Reply.push_back(date_tool_reply());
      }
    }

  return ret_submitted;
}

// ------------------------------------------------------------------------- //
// System Help

nlohmann::json TOOL_FUNCTIONS::system_help_tool()
{
  return 
  {
    {"type", "function"},
    {"function", {
      {"name", "system_help_tool"},
      {"description", "The user is asking for help on how to interact with the system."}
      //{"parameters", {}}
    }}
  };

}

nlohmann::json TOOL_FUNCTIONS::system_help_reply(THOUGHT_VARIABLES &Thought_Control)
{
  return
  {
    {"role", "tool"},
    {"content", Thought_Control.MEMORY.FILE_MANAGER.get_file("system_help")}, 
    {"name", "system_help_tool"}
  };
}

bool TOOL_FUNCTIONS::SYSTEM_HELP_CALL(nlohmann::json Message, nlohmann::json &Tool_Reply, THOUGHT_VARIABLES &Thought_Control)
{
  bool ret_submitted = false;
  if (Message["tool_calls"]["function"]["name"] == "system_help_tool")
  {
    ret_submitted = true;
    if(SYSTEM_HELP.enabled())
    {
      Tool_Reply.push_back(build_new_tool_reply_start(Message));
      Tool_Reply.push_back(system_help_reply(Thought_Control));
    }
  }

  return ret_submitted;
}

// ------------------------------------------------------------------------- //
// Memory Files

nlohmann::json TOOL_FUNCTIONS::memory_file_list_tool()
{
  return 
  {
    {"type", "function"},
    {"function", {
      {"name", "memory_file_list_tool"},
      {"description", "The user is asking for the memory file list."}
    }}
  };

}

nlohmann::json TOOL_FUNCTIONS::memory_file_list_reply(THOUGHT_VARIABLES &Thought_Control)
{
  return
  {
    {"role", "tool"},
    {"content", "Memory file list: " + 
      Thought_Control.MEMORY.FILE_MANAGER.memory_file_list()}, 
    {"name", "memory_file_list_tool"}
  };
}

bool TOOL_FUNCTIONS::MEMORY_FILES_LIST_CALL(nlohmann::json Message, nlohmann::json &Tool_Reply, THOUGHT_VARIABLES &Thought_Control)
{
  bool ret_submitted = false;
  if (Message["tool_calls"]["function"]["name"] == "memory_file_list_tool")
  {
    ret_submitted = true;
    if(MEMORY_FILES_LIST.enabled())
    {

      Tool_Reply.push_back(build_new_tool_reply_start(Message));
      Tool_Reply.push_back(memory_file_list_reply(Thought_Control));
    }
  }

  return ret_submitted;
}

// ------------------------------------------------------------------------- //
// Memory Files Print

nlohmann::json TOOL_FUNCTIONS::memory_file_print_tool()
{
  return 
  {
    {"type", "function"},
    {"function", {
      {"name", "memory_file_print"},
      {"description", "Print the contents of a memory file."},
      {"parameters", {
        {"type", "object"},
        {"properties", {
          {"name", {
            {"type", "string"},
            {"description", "The name of the memory file to print."}
          }}
        }},
        {"required", {"name"}}
      }}
    }}
  };
}

nlohmann::json TOOL_FUNCTIONS::memory_file_print_reply(THOUGHT_VARIABLES &Thought_Control)
{
  //if (Memory.FILE_MANAGER.is_file_ready(MEMORY_FILES_PRINT_NAME))
  {
    return
    {
      {"role", "tool"},
      {"content", "The contents of the memory file " + 
         MEMORY_FILES_PRINT_NAME + 
         " is: \n" + 
         Thought_Control.MEMORY.FILE_MANAGER.get_file(MEMORY_FILES_PRINT_NAME)}, 
      {"name", "memory_file_print"}
    };
  }
  /*
  else
  {
    return
    {
      {"role", "tool"},
      {"content", "The memory file: " + 
         MEMORY_FILES_PRINT_NAME + 
         " is not found.\n"}, 
      {"name", "memory_file_print"}
    };
  }
  */
}

bool TOOL_FUNCTIONS::MEMORY_FILES_PRINT_CALL(nlohmann::json Message, nlohmann::json &Tool_Reply, THOUGHT_VARIABLES &Thought_Control)
{
  bool ret_submitted = false;
  if (Message["tool_calls"]["function"]["name"] == "memory_file_print")
  {
    ret_submitted = true;

    if(MEMORY_FILES_PRINT.enabled())
    {

      if (Message["tool_calls"]["function"].contains("arguments"))
      {
        if (Message["tool_calls"]["function"]["arguments"].contains("name"))
        {
          MEMORY_FILES_PRINT_NAME = Message["tool_calls"]["function"]["arguments"]["name"];

          Tool_Reply.push_back(build_new_tool_reply_start(Message));
          Tool_Reply.push_back(memory_file_print_reply(Thought_Control));
        }
      }
    }
  }

  return ret_submitted;
}

// ------------------------------------------------------------------------- //
// Maintenance Mode

// This just doesnt work
/*
nlohmann::json TOOL_FUNCTIONS::maintenance_mode_enter_tool()
{
  return 
  {
    {"type", "function"},
    {"function", {
      {"name", "maintenance_mode_enter"},
      {"description", "The user is asking to enter the maintenance mode"}
      //{"parameters", {}}
    }}
  };
}

nlohmann::json TOOL_FUNCTIONS::maintenance_mode_enter_reply(THOUGHT_VARIABLES &Thought_Control)
{

  Thought_Control.TRAIN_OF_THOUGH.back().ABOUT = "maintenance mode cycle";
  Thought_Control.TRAIN_OF_THOUGH.back().THINKING_STAGE = 0;
  
  return
  {
    {"role", "tool"},
    {"content", "Entering maintenance mode."}, 
    {"name", "maintenance_mode_enter"}
  };
}

bool TOOL_FUNCTIONS::MAINTENANCE_MODE_ENTER_CALL(nlohmann::json Message, nlohmann::json &Tool_Reply, THOUGHT_VARIABLES &Thought_Control)
{
  bool ret_submitted = false;

  if (Message["tool_calls"]["function"]["name"] == "maintenance_mode_enter")
  {
    ret_submitted = true;

    Tool_Reply.push_back(build_new_tool_reply_start(Message));
    Tool_Reply.push_back(maintenance_mode_enter_reply(Thought_Control));
    dump_string(DUMP_DIRECTORY, "test.json", Tool_Reply.dump(2));
  }

  return ret_submitted;
}
*/

// ------------------------------------------------------------------------- //



// ------------------------------------------------------------------------- //
// Memory Files Edit
/*
nlohmann::json TOOL_FUNCTIONS::memory_file_edit_show_name_tool()
{
  return 
  {
    {"type", "function"},
    {"function", {
      {"name", "memory_file_edit_show_name_tool"},
      {"description", "Show the name of the memory file that is being worked on."},
      {"parameters", {
        {"type", "object"},
        {"properties", {
          {"name", {
            {"type", "string"},
            {"description", "The name of the memory file being worked on."}
          }}
        }}
      }}
    }}
  };
}

bool TOOL_FUNCTIONS::memory_file_edit_show_name_call(nlohmann::json Message, nlohmann::json &Tool_Reply)
{
  bool ret_submitted = false;

  if (Message["tool_calls"]["function"]["name"] == "memory_file_edit_show_name_tool")
  {
    ret_submitted = true;

    Tool_Reply.push_back(build_new_tool_reply_start(Message));
    Tool_Reply.push_back(memory_file_edit_show_name_reply());
  }

  return ret_submitted;
}

nlohmann::json TOOL_FUNCTIONS::memory_file_edit_show_name_reply()
{
  {
    return
    {
      {"role", "tool"},
      {"content", "The name of the memory file being worked is: " + 
        EDIT_MEMORY_FILE_NAME + 
        "\n"}, 
      {"name", "memory_file_edit_show_name_tool"}
    };
  }
}
*/

nlohmann::json TOOL_FUNCTIONS::memory_file_edit_save_tool()
{
  return 
  {
    {"type", "function"},
    {"function", {
      {"name", "memory_file_edit_save_tool"},
      {"description", "Store or save the memory file text with the filename provided."},
      {"parameters", {
        {"type", "object"},
        {"properties", {
          {"file_name", {
            {"type", "string"},
            {"description", "The name of the memory file without any spaces or special characters."}
          }},
          {"file_content", {
            {"type", "string"},
            {"description", "The Contentent of a memory file."}
          }}
        }},
        {"required", {"file_name", "file_content"}}
      }}
    }}
  };
}






nlohmann::json TOOL_FUNCTIONS::memory_file_edit_save_reply(THOUGHT_VARIABLES &Thought_Control)
{
  MEMORY_FILE tmp_memory_file;
  tmp_memory_file.TITLE = EDIT_MEMORY_FILE_NAME;
  tmp_memory_file.CONTENT = EDIT_MEMORY_FILE_CONTENT;
  Thought_Control.MEMORY.FILE_MANAGER.add_file(tmp_memory_file);

  {
    return
    {
      {"role", "tool"},
      {"content", "The memory file was saved."}, 
      {"name", "memory_file_edit_save_tool"}
    };
  }

}

bool TOOL_FUNCTIONS::memory_file_edit_save_call(nlohmann::json Message, nlohmann::json &Tool_Reply, THOUGHT_VARIABLES &Thought_Control)
{
  bool ret_submitted = false;

  if (Message["tool_calls"]["function"]["name"] == "memory_file_edit_save_tool")
  {
    ret_submitted = true;

    if(MEMORY_FILES_EDIT.enabled())
    {
      if (Message["tool_calls"]["function"].contains("arguments"))
      {
        if (Message["tool_calls"]["function"]["arguments"].contains("file_name") && 
            Message["tool_calls"]["function"]["arguments"].contains("file_content"))
        {
          EDIT_MEMORY_FILE_NAME = Message["tool_calls"]["function"]["arguments"]["file_name"];
          EDIT_MEMORY_FILE_CONTENT = Message["tool_calls"]["function"]["arguments"]["file_content"];
          
          Tool_Reply.push_back(build_new_tool_reply_start(Message));
          Tool_Reply.push_back(memory_file_edit_save_reply(Thought_Control));
          dump_string(DUMP_DIRECTORY, "test.json", Tool_Reply.dump(2));
        }
      }
    }
  }

  return ret_submitted;

}


// ------------------------------------------------------------------------- //
// UNKNOWN CALL

nlohmann::json TOOL_FUNCTIONS::unknown_reply(string Function_Name)
{
  return
  {
    {"role", "tool"},
    {"content", "Function not found: " + Function_Name}, 
    {"name", Function_Name}
  };
}

bool TOOL_FUNCTIONS::UNKNOWN_CALL(nlohmann::json Message, nlohmann::json &Tool_Reply)
{
  bool ret_submitted = true;

  string unknown_function = Message["tool_calls"]["function"]["name"];

  Tool_Reply.push_back(build_new_tool_reply_start(Message));
  Tool_Reply.push_back(unknown_reply(unknown_function));

  return ret_submitted;
}

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //

nlohmann::json TOOL_FUNCTIONS::tool_list()
{
  nlohmann::json ret_tools_list;

  // ------------------------------------------------------------------------- //
  // Weather from testing,  always responds the same.
  if (WEATHER_TOOL.enabled())
  {
    ret_tools_list.push_back(weather_tool());
  }

  // ------------------------------------------------------------------------- //
  // Date and TImestamp
  if (CLOCK_TOOL.enabled())
  {
    ret_tools_list.push_back(clock_tool());
  }

  if (DATE_TOOL.enabled())
  {
    ret_tools_list.push_back(date_tool());
  }

  // ------------------------------------------------------------------------- //
  // System Help
  if (SYSTEM_HELP.enabled())
  {
    ret_tools_list.push_back(system_help_tool());
  }

  // ------------------------------------------------------------------------- //
  // Memory Files
  if (MEMORY_FILES_LIST.enabled())
  {
    ret_tools_list.push_back(memory_file_list_tool());
  }

  if (MEMORY_FILES_PRINT.enabled())
  {
    ret_tools_list.push_back(memory_file_print_tool());
  }
  
  // ------------------------------------------------------------------------- //
  // Memory Files Edit

  if (MEMORY_FILES_EDIT.enabled())
  {
    ret_tools_list.push_back(memory_file_edit_save_tool());
  }

  // ------------------------------------------------------------------------- //
  // ------------------------------------------------------------------------- //
  // Maintenance Mode
  /*
  if (MAINTENANCE_MODE_ENTER.enabled())
  {
    ret_tools_list.push_back(maintenance_mode_enter_tool());
  }
  */

  // ------------------------------------------------------------------------- //
  return ret_tools_list;
}

// ------------------------------------------------------------------------- //

void TOOL_FUNCTIONS::enable_ollama()
{
  WEATHER_TOOL.enable_set(true);
  CLOCK_TOOL.enable_set(true);
  DATE_TOOL.enable_set(true);
  SYSTEM_HELP.enable_set(true);
  MEMORY_FILES_LIST.enable_set(true);
  MEMORY_FILES_PRINT.enable_set(false);
  MEMORY_FILES_EDIT.enable_set(false);
}

void TOOL_FUNCTIONS::enable_mainteance_mode()
{
  WEATHER_TOOL.enable_set(false);
  CLOCK_TOOL.enable_set(false);
  DATE_TOOL.enable_set(false);
  SYSTEM_HELP.enable_set(false);
  MEMORY_FILES_LIST.enable_set(false);
  MEMORY_FILES_PRINT.enable_set(false);
  MEMORY_FILES_EDIT.enable_set(false);
}

void TOOL_FUNCTIONS::enable_memory_file_mode()
{
  WEATHER_TOOL.enable_set(false);
  CLOCK_TOOL.enable_set(false);
  DATE_TOOL.enable_set(false);
  SYSTEM_HELP.enable_set(false);
  MEMORY_FILES_LIST.enable_set(true);
  MEMORY_FILES_PRINT.enable_set(true);
  MEMORY_FILES_EDIT.enable_set(true);
}

#endif // TOOL_FUNCTIONS_CPP
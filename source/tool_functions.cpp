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
      {"description", "Get the time"}
      //{"parameters", {}}
    }}
  };
}

nlohmann::json TOOL_FUNCTIONS::clock_tool_reply()
{
  return
  {
    {"role", "tool"},
    {"content", "The time in military timeis " + current_time() + "."}, 
    {"name", "get_current_time"}
  };
}

bool TOOL_FUNCTIONS::CLOCK_TOOL_CALL(nlohmann::json Message, nlohmann::json &Tool_Reply)
{
  bool ret_submitted = false;

  if (Message["tool_calls"]["function"]["name"] == "get_current_time")
  {
    ret_submitted = true;

    Tool_Reply.push_back(build_new_tool_reply_start(Message));
    Tool_Reply.push_back(clock_tool_reply());
    dump_string(DUMP_DIRECTORY, "test.json", Tool_Reply.dump(2));
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
      {"description", "Get the date"}
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

    Tool_Reply.push_back(build_new_tool_reply_start(Message));
    Tool_Reply.push_back(date_tool_reply());
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

nlohmann::json TOOL_FUNCTIONS::system_help_reply(REMEMBER &Memory)
{
  return
  {
    {"role", "tool"},
    {"content", Memory.FILE_MANAGER.get_file("system_help")}, 
    {"name", "system_help_tool"}
  };
}

bool TOOL_FUNCTIONS::SYSTEM_HELP_CALL(nlohmann::json Message, nlohmann::json &Tool_Reply, REMEMBER &Memory)
{
  bool ret_submitted = false;

  if (Message["tool_calls"]["function"]["name"] == "system_help_tool")
  {
    ret_submitted = true;

    Tool_Reply.push_back(build_new_tool_reply_start(Message));
    Tool_Reply.push_back(system_help_reply(Memory));
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

nlohmann::json TOOL_FUNCTIONS::memory_file_list_reply(REMEMBER &Memory)
{
  return
  {
    {"role", "tool"},
    {"content", "Memory file list: " + 
                Memory.FILE_MANAGER.memory_file_list()}, 
    {"name", "memory_file_list_tool"}
  };
}

bool TOOL_FUNCTIONS::MEMORY_FILES_LIST_CALL(nlohmann::json Message, nlohmann::json &Tool_Reply, REMEMBER &Memory)
{
  bool ret_submitted = false;

  if (Message["tool_calls"]["function"]["name"] == "memory_file_list_tool")
  {
    ret_submitted = true;

    Tool_Reply.push_back(build_new_tool_reply_start(Message));
    Tool_Reply.push_back(memory_file_list_reply(Memory));
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

nlohmann::json TOOL_FUNCTIONS::memory_file_print_reply(REMEMBER &Memory)
{
  //if (Memory.FILE_MANAGER.is_file_ready(MEMORY_FILES_PRINT_NAME))
  {
    return
    {
      {"role", "tool"},
      {"content", "The contents of the memory file " + 
         MEMORY_FILES_PRINT_NAME + 
         " is: \n" + 
         Memory.FILE_MANAGER.get_file(MEMORY_FILES_PRINT_NAME)}, 
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

bool TOOL_FUNCTIONS::MEMORY_FILES_PRINT_CALL(nlohmann::json Message, nlohmann::json &Tool_Reply, REMEMBER &Memory)
{
  bool ret_submitted = false;

  if (Message["tool_calls"]["function"]["name"] == "memory_file_print")
  {
    ret_submitted = true;

    if (Message["tool_calls"]["function"].contains("arguments"))
    {
      if (Message["tool_calls"]["function"]["arguments"].contains("name"))
      {
        MEMORY_FILES_PRINT_NAME = Message["tool_calls"]["function"]["arguments"]["name"];

        Tool_Reply.push_back(build_new_tool_reply_start(Message));
        Tool_Reply.push_back(memory_file_print_reply(Memory));
      }
    }
  }

  return ret_submitted;
}

// ------------------------------------------------------------------------- //
// Memory Files

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
  return ret_tools_list;
}

// ------------------------------------------------------------------------- //

#endif // TOOL_FUNCTIONS_CPP
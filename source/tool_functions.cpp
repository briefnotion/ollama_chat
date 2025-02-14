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

// ------------------------------------------------------------------------- //
// Memory Files

nlohmann::json TOOL_FUNCTIONS::memory_file_list_tool()
{
  return 
  {
    {"type", "function"},
    {"function", {
      {"name", "memory_file_list_tool"},
      {"description", "The user is asking for the memory file list."},
      {"parameters", {}}
    }}
  };

}

nlohmann::json TOOL_FUNCTIONS::memory_file_list_reply(REMEMBER &Memory)
{
  return
  {
    {"role", "tool"},
    {"content", "Memory file list:" + 
                Memory.FILE_MANAGER.memory_file_list()}, 
    {"name", "memory_file_list_tool"}
  };

}

// ------------------------------------------------------------------------- //
// Memory Files

nlohmann::json TOOL_FUNCTIONS::tool_list()
{
  nlohmann::json ret_tools_list;

  if (WEATHER_TOOL_SUBMIT.enabled())
  {
    ret_tools_list.push_back(weather_tool());
  }

  if (CLOCK_TOOL_SUBMIT.enabled())
  {
    ret_tools_list.push_back(clock_tool());
  }

  if (DATE_TOOL_SUBMIT.enabled())
  {
    ret_tools_list.push_back(date_tool());
  }

  if (SYSTEM_HELP_SUBMIT.enabled())
  {
    ret_tools_list.push_back(system_help_tool());
  }

  if (MEMORY_FILES_SUBMIT.enabled())
  {
    ret_tools_list.push_back(memory_file_list_tool());
  }

  return ret_tools_list;
}

// ------------------------------------------------------------------------- //

#endif // TOOL_FUNCTIONS_CPP
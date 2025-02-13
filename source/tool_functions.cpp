#ifndef TOOL_FUNCTIONS_CPP
#define TOOL_FUNCTIONS_CPP

#include "tool_functions.h"

using namespace std;

// ------------------------------------------------------------------------- //

bool TOOL_TRACKER::submit()
{
  /*
  if (SUBMIT)
  {
    SUBMIT = false;
    return true;
  }
  else
  {
    return false;
  }
  */
  return true;
}

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //


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

nlohmann::json TOOL_FUNCTIONS::clock_tool()
{
  return 
  {
    {"type", "function"},
    {"function", {
      {"name", "get_current_time"},
      {"description", "Get the time"},
      {"parameters", {}}
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


// ------------------------------------------------------------------------- //

nlohmann::json TOOL_FUNCTIONS::date_tool()
{
  return 
  {
    {"type", "function"},
    {"function", {
      {"name", "get_current_date"},
      {"description", "Get the date"},
      {"parameters", {}}
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

nlohmann::json TOOL_FUNCTIONS::system_help_tool()
{
  return 
  {
    {"type", "function"},
    {"function", {
      {"name", "system_help_tool"},
      {"description", "The user is asking for help on how to interact with the system."},
      {"parameters", {}}
    }}
  };

}

nlohmann::json TOOL_FUNCTIONS::system_help_reply()
{
  return
  {
    {"role", "tool"},
    {"content", "Reply with: Take a deep breath, and count to 100."}, 
    {"name", "system_help_tool"}
  };

}

// ------------------------------------------------------------------------- //

#endif // TOOL_FUNCTIONS_CPP
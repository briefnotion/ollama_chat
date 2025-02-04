#ifndef TOOL_FUNCTIONS_CPP
#define TOOL_FUNCTIONS_CPP

#include "tool_functions.h"

using namespace std;

// ------------------------------------------------------------------------- //

bool TOOL_TRACKER::submit()
{
  if (SUBMIT)
  {
    SUBMIT = false;
    return true;
  }
  else
  {
    return false;
  }
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
  return
  {
    {"role", "tool"},
    {"content", "The current weather in " + WEATHER_TOOL_PARAM_LOCATION + " has a temperature of 25° " + WEATHER_TOOL_PARAM_FORMAT + "."}, 
    {"name", "get_current_weather"}
  };
}

// ------------------------------------------------------------------------- //

nlohmann::json TOOL_FUNCTIONS::clock_tool()
{
  return 
  {
    {"type", "function"},
    {"function", {
      {"name", "get_current_time"},
      {"description", "Get the current time"}
    }}
  };

}

nlohmann::json TOOL_FUNCTIONS::clock_tool_reply()
{
  return
  {
    {"role", "tool"},
    {"content", "The current time is  " + current_time() + "."}, 
    {"name", "get_current_time"}
  };

}

// ------------------------------------------------------------------------- //

#endif // TOOL_FUNCTIONS_CPP
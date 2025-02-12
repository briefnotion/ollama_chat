#ifndef TOOL_FUNCTIONS_H
#define TOOL_FUNCTIONS_H

#include <iostream>
#include <string>

#include <nlohmann/json.hpp>

#include "helper.h"

// ------------------------------------------------------------------------- //

class TOOL_TRACKER
{
  private:

  bool SUBMIT = true;

  public:
  bool submit();

};

// ------------------------------------------------------------------------- //

class TOOL_FUNCTIONS
{
  private:



  public:

  // Weather Tool - For Testing
  TOOL_TRACKER WEATHER_TOOL_SUBMIT;
  nlohmann::json weather_tool();
  string WEATHER_TOOL_PARAM_FORMAT = "";
  string WEATHER_TOOL_PARAM_LOCATION = "";
  nlohmann::json weather_tool_reply();

  // Clock Tool
  TOOL_TRACKER CLOCK_TOOL_SUBMIT;
  nlohmann::json clock_tool();
  nlohmann::json clock_tool_reply();

  // Date Tool
  TOOL_TRACKER DATE_TOOL_SUBMIT;
  nlohmann::json date_tool();
  nlohmann::json date_tool_reply();

  // Get Help File
  TOOL_TRACKER SYSTEM_HELP_SUBMIT;
  nlohmann::json system_help_tool();
  nlohmann::json system_help_reply();

};

// ------------------------------------------------------------------------- //

#endif // TOOL_FUNCTIONS_H
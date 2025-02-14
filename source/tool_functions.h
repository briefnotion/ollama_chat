#ifndef TOOL_FUNCTIONS_H
#define TOOL_FUNCTIONS_H

#include <iostream>
#include <string>

#include <nlohmann/json.hpp>

#include "helper.h"
#include "remember.h"

// ------------------------------------------------------------------------- //

class TOOL_TRACKER
{
  private:

  bool ENABLED = true;

  public:
  bool enabled();
  void enable_set(bool Enable);

};

// ------------------------------------------------------------------------- //

class TOOL_FUNCTIONS
{
  private:



  public:

  // Weather from testing,  always responds the same.
  TOOL_TRACKER WEATHER_TOOL_SUBMIT;
  nlohmann::json weather_tool();
  string WEATHER_TOOL_PARAM_FORMAT = "";
  string WEATHER_TOOL_PARAM_LOCATION = "";
  nlohmann::json weather_tool_reply();

  // Date and TIme
  TOOL_TRACKER CLOCK_TOOL_SUBMIT;
  nlohmann::json clock_tool();
  nlohmann::json clock_tool_reply();

  // Date and TIme
  TOOL_TRACKER DATE_TOOL_SUBMIT;
  nlohmann::json date_tool();
  nlohmann::json date_tool_reply();

  // System Help
  TOOL_TRACKER SYSTEM_HELP_SUBMIT;
  nlohmann::json system_help_tool();
  nlohmann::json system_help_reply(REMEMBER &Memory);

  // Memory Files
  TOOL_TRACKER MEMORY_FILES_SUBMIT;
  nlohmann::json memory_file_list_tool();
  nlohmann::json memory_file_list_reply(REMEMBER &Memory);

  // Tool Call List
  nlohmann::json tool_list();
};

// ------------------------------------------------------------------------- //

#endif // TOOL_FUNCTIONS_H
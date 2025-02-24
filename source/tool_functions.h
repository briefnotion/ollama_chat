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

  nlohmann::json build_new_tool_reply_start(nlohmann::json Message);

  public:

  // Weather from testing,  always responds the same.
  TOOL_TRACKER WEATHER_TOOL;
  nlohmann::json weather_tool();
  string WEATHER_TOOL_PARAM_FORMAT = "";
  string WEATHER_TOOL_PARAM_LOCATION = "";
  nlohmann::json weather_tool_reply();
  bool WEATHER_TOOL_CALL(nlohmann::json Message, nlohmann::json &Tool_Reply);

  // Date and TIme
  TOOL_TRACKER CLOCK_TOOL;
  nlohmann::json clock_tool();
  nlohmann::json clock_tool_reply();
  bool CLOCK_TOOL_CALL(nlohmann::json Message, nlohmann::json &Tool_Reply);

  TOOL_TRACKER DATE_TOOL;
  nlohmann::json date_tool();
  nlohmann::json date_tool_reply();
  bool DATE_TOOL_CALL(nlohmann::json Message, nlohmann::json &Tool_Reply);

  // System Help
  TOOL_TRACKER SYSTEM_HELP;
  nlohmann::json system_help_tool();
  nlohmann::json system_help_reply(REMEMBER &Memory);
  bool SYSTEM_HELP_CALL(nlohmann::json Message, nlohmann::json &Tool_Reply, REMEMBER &Memory);

  // Memory Files
  TOOL_TRACKER MEMORY_FILES_LIST;
  nlohmann::json memory_file_list_tool();
  nlohmann::json memory_file_list_reply(REMEMBER &Memory);
  bool MEMORY_FILES_LIST_CALL(nlohmann::json Message, nlohmann::json &Tool_Reply, REMEMBER &Memory);
  
  TOOL_TRACKER MEMORY_FILES_PRINT;
  nlohmann::json memory_file_print_tool();
  string MEMORY_FILES_PRINT_NAME = "";
  nlohmann::json memory_file_print_reply(REMEMBER &Memory);
  bool MEMORY_FILES_PRINT_CALL(nlohmann::json Message, nlohmann::json &Tool_Reply, REMEMBER &Memory);
  
  // ------------------------------------------------------------------------- //
  // Tool Call List
  nlohmann::json tool_list();
};

// ------------------------------------------------------------------------- //

#endif // TOOL_FUNCTIONS_H
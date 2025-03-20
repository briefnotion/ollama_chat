#ifndef TOOL_FUNCTIONS_H
#define TOOL_FUNCTIONS_H

#include <iostream>
#include <string>

#include <nlohmann/json.hpp>

#include "helper.h"
#include "thought_variables.h"

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

  // ------------------------------------------------------------------------- //
  // Weather from testing,  always responds the same.
  TOOL_TRACKER WEATHER_TOOL;
  nlohmann::json weather_tool();
  string WEATHER_TOOL_PARAM_FORMAT = "";
  string WEATHER_TOOL_PARAM_LOCATION = "";
  nlohmann::json weather_tool_reply();
  bool WEATHER_TOOL_CALL(nlohmann::json Message, nlohmann::json &Tool_Reply);

  // ------------------------------------------------------------------------- //
  // Date and TIme
  TOOL_TRACKER CLOCK_TOOL;
  nlohmann::json clock_tool();
  string CLOCK_TOOL_PARAM_FORMAT = "";
  nlohmann::json clock_tool_reply();
  bool CLOCK_TOOL_CALL(nlohmann::json Message, nlohmann::json &Tool_Reply);

  TOOL_TRACKER DATE_TOOL;
  nlohmann::json date_tool();
  nlohmann::json date_tool_reply();
  bool DATE_TOOL_CALL(nlohmann::json Message, nlohmann::json &Tool_Reply);

  // ------------------------------------------------------------------------- //
  // System Help
  TOOL_TRACKER SYSTEM_HELP;
  nlohmann::json system_help_tool();
  nlohmann::json system_help_reply(THOUGHT_VARIABLES &Thought_Control);
  bool SYSTEM_HELP_CALL(nlohmann::json Message, nlohmann::json &Tool_Reply, THOUGHT_VARIABLES &Thought_Control);

  // ------------------------------------------------------------------------- //
  // Memory Files
  TOOL_TRACKER MEMORY_FILES_LIST;
  nlohmann::json memory_file_list_tool();
  nlohmann::json memory_file_list_reply(THOUGHT_VARIABLES &Thought_Control);
  bool MEMORY_FILES_LIST_CALL(nlohmann::json Message, nlohmann::json &Tool_Reply, THOUGHT_VARIABLES &Thought_Control);
  
  TOOL_TRACKER MEMORY_FILES_PRINT;
  nlohmann::json memory_file_print_tool();
  string MEMORY_FILES_PRINT_NAME = "";
  nlohmann::json memory_file_print_reply(THOUGHT_VARIABLES &Thought_Control);
  bool MEMORY_FILES_PRINT_CALL(nlohmann::json Message, nlohmann::json &Tool_Reply, THOUGHT_VARIABLES &Thought_Control);

  // ------------------------------------------------------------------------- //
  // Memory Files Edit
  TOOL_TRACKER MEMORY_FILES_EDIT;
  
  string EDIT_MEMORY_FILE_NAME = "";
  string EDIT_MEMORY_FILE_CONTENT = "";

  //nlohmann::json memory_file_edit_show_name_tool();
  //bool memory_file_edit_show_name_call(nlohmann::json Message, nlohmann::json &Tool_Reply);
  //nlohmann::json memory_file_edit_show_name_reply();
  
  /*
  nlohmann::json memory_file_edit_store_name_tool();
  bool memory_file_edit_store_name_call(nlohmann::json Message, nlohmann::json &Tool_Reply, THOUGHT_VARIABLES &Thought_Control);
  nlohmann::json memory_file_edit_store_name_reply(THOUGHT_VARIABLES &Thought_Control);

  nlohmann::json memory_file_edit_show_content_tool();
  bool memory_file_edit_show_content_call(nlohmann::json Message, nlohmann::json &Tool_Reply, THOUGHT_VARIABLES &Thought_Control);
  nlohmann::json memory_file_edit_show_content_reply(THOUGHT_VARIABLES &Thought_Control);
  
  nlohmann::json memory_file_edit_store_content_tool();
  bool memory_file_edit_store_content_call(nlohmann::json Message, nlohmann::json &Tool_Reply, THOUGHT_VARIABLES &Thought_Control);
  nlohmann::json memory_file_edit_store_content_reply(THOUGHT_VARIABLES &Thought_Control);
  */

  nlohmann::json memory_file_edit_save_tool();
  nlohmann::json memory_file_edit_save_reply(THOUGHT_VARIABLES &Thought_Control);
  bool memory_file_edit_save_call(nlohmann::json Message, nlohmann::json &Tool_Reply, THOUGHT_VARIABLES &Thought_Control);

  // ------------------------------------------------------------------------- //
  nlohmann::json unknown_reply(string Function_Name);
  bool UNKNOWN_CALL(nlohmann::json Message, nlohmann::json &Tool_Reply);

  // Maintenance Mode
  /*
  TOOL_TRACKER MAINTENANCE_MODE_ENTER;
  nlohmann::json maintenance_mode_enter_tool();
  nlohmann::json maintenance_mode_enter_reply(THOUGHT_VARIABLES &Thought_Control);
  bool MAINTENANCE_MODE_ENTER_CALL(nlohmann::json Message, nlohmann::json &Tool_Reply, THOUGHT_VARIABLES &Thought_Control);
  */
  
  // ------------------------------------------------------------------------- //
  // Tool Call List
  nlohmann::json tool_list();

  // Tool Enable Block
  void enable_ollama();
  void enable_mainteance_mode();
  void enable_memory_file_mode();
};

// ------------------------------------------------------------------------- //

#endif // TOOL_FUNCTIONS_H
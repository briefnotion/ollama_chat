#ifndef OLLAMA_API_PYTHON_H
#define OLLAMA_API_PYTHON_H

#include <iostream>
#include <string>
#include <vector>

//#include "ollama.hpp"   // Include this for: Fatal glibc error: pthread_mutex_lock.c:94 (___pthread_mutex_lock): assertion failed: mutex->__data.__owner == 0
#include <nlohmann/json.hpp>

#include "helper.h"
#include "tty_io.h"
#include "threading.h"

#include "rasapi.h"
#include "tool_functions.h"
#include "thought_variables.h"

#define OLLAMA_SERVER_NOT_CONNECTED     0
#define OLLAMA_SERVER_CONNECTED         1
#define OLLAMA_SERVER_CONNECTION_FAILED -1

#define OLLAMA_RESPONSE_THREAD_TIMER_DELAY   60   // This will be in frames per second (fps)

// OLLAMA_API_STATUS is used to track the status of the API
#define OLLAMA_API_READY_FOR_REQUEST      0
//#define OLLAMA_API_WRITING_REQUEST        1
//#define OLLAMA_API_REQUEST_SUBMITTED      2 
#define OLLAMA_API_RESPONS_GENERATING     3
#define OLLAMA_API_RESPONSE_DONE          4
#define OLLAMA_API_READY_TO_GATHER        5

// Roles
#define ROLE_SYSTEM     "system"
#define ROLE_ASSISTANT  "assistant"
#define ROLE_USER       "user"

// Role user names
#define USER_AI           ""
#define USER_DOCUMENATION "DOCUMENTATION"
#define USER_INSTRUCTIONS "INSTRUCTIONNS"

using namespace std;

// ------------------------------------------------------------------------- //

class OLLAMA_API_MUTEX_PYTHON
{
  private:

  mutable mutex MUTEX_;

  vector<string> RESPONSE_VECTOR;
  
  // Complete text response
  string COMPLETE_RESPONSE = "";
  //nlohmann::json RESPONSE;

  int DONE = 0;

  string COMMAND_LINE = "";

  // Full JSON Response after done.
  nlohmann::json COMPLETE_RESPONSE_AFTER_DONE;
  bool COMPLETE_RESPONSE_READY_AFTER_DONE = false;

  bool CHANGED = false;

  public:

  // Submitting Question
  void set_command_line(string Command);
  string command_line() const;

  // Getting a copy of the text response afte it is done.
  string get_complete_response() const;

  // Getting a copy of the response afte it is done.
  void set_complete_response_after_done(nlohmann::json Response);

  nlohmann::json get_complete_response_after_done();

  bool complete_response_ready_after_done() const;

  // Generating Response Complete
  int done() const;
  void set_done(int Done);  

  // Text Response Generating
  void add_to_response(const string& value);
  int response_size() const;
  void get_response_to_vector(vector<string> &Receiving_Vector);
};

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //

class MESSAGE_PYTHON
{
  public:
  string role;
  string content;
};

// ------------------------------------------------------------------------- //

class OLLAMA_API_PYTHON_PROPS
{
  public:

  // Model
  string MODEL = "";  // "llama3.1:8b"
  string URL   = "";  // "http://localhost:11434"
  
  nlohmann::json OLLAMA_OPTIONS;

  // Request as Generate or Chat
  int REQUEST_MODE = 0; // 0: Chat, 1: Generate

  // Command Line Input
  string BASH_SHELL =                   "bash -c '";
  string ENVIRONMENT =                  "source /home/briefn/py/venv/bin/activate";
  /*
  string SCRIPT_SEARCH =                "python3 ../python/search.py ";
  string SCRIPT_SEARCH_DOCS_ONLY =      "python3 ../python/search_docs_only.py";
  string SCRIPT_SEARCH_DOCS_ONLY_COS =  "python3 ../python/search_docs_only_cos.py ";
  string SCRIPT_EMBED_FILE =            "python3 ../python/import_sp.py";
  string SCRIPT_CLEAR_DATABASE =        "python3 ../python/clear_database.py";
  string SCRIPT_LIST_DATABASE =         "python3 ../python/list_database.py";
  */

  string TEST =     "python3 ../python/test.py";
  //string REQUEST =  "python3 ../python/generate_request.py";
  //string REQUEST =  "python3 ../python/generate_request_2.py";
  //string REQUEST =  "python3 ../python/generate_request_3.py";
  string REQUEST_CHAT     =  "python3 ../python/generate_request_4.py";
  string REQUEST_GENERATE =  "python3 ../python/generate_request_5.py";

  string REQUEST_JSON_FILENAME = "request.json";
  string RESPONSE_JSON_FILENAME = "response.json";

};

class OLLAMA_API_PYTHON
{
  // ------------------------------------------------------------------------- //
  // This section reserved for async
  private:
  
  OLLAMA_API_MUTEX_PYTHON OLLAMA_MUTEX;

  THREADING_INFO  PYTHON_QUESTION_RESPONSE_THREAD;

  // Constants for building the command line
  const string aa = " && ";
  const string sp = " ";

  string exec(const char* cmd);
  void exec_thread_question();
  void thread();

  int CONNECTION_STATUS = 0;
  bool CONNECTION_STATUS_CHANGED = true;

  //std::function<void(const ollama::response&)> response_callback;
  //void on_receive_response(const ollama::response& Request);

  public:

  void proc_render_thread();
  // ------------------------------------------------------------------------- //

  private:
  //Ollama OLLAMA;

  vector<string> OLLAMA_MODELS_LIST;
  vector<string> OLLAMA_MODELS_RUNNING;

  //ollama::options OPTIONS;

  nlohmann::json RESPONSE;

  // CHAT MODEL
  nlohmann::json CONVERSATION;
  nlohmann::json CONVERSATION_CONTEXT;

  nlohmann::json CONVERSATION_SNAP_SHOT;
  string ROLE_1_SNAP_SHOT = "";
  string NAME_1_SNAP_SHOT = "";
  string QUESTION_1_SNAP_SHOT = "";
  string ROLE_2_SNAP_SHOT = "";
  string NAME_2_SNAP_SHOT = "";
  string QUESTION_2_SNAP_SHOT = "";

  //nlohmann::json CONTEXT;
  vector<nlohmann::json> CONTEXT_PAUSED;

  // Different types of ask.
  bool ALLOW_OUTPUT     = true;
  bool CONSIDER_CONTEXT = true;
  bool REMEMBER_CONTEXT = true;

  public:

  OLLAMA_API_PYTHON_PROPS PROPS;

  THREADING_INFO  OLLAMA_RESPONSE_THREAD;

  vector<string> RESPONSE_STRING_VECTOR;
  string RESPONSE_FULL = "";  // RESPONSE.as_simple_string(); not working so
                              //  as fall back

  // Sending
  nlohmann::json REQUEST = "";

  // Tool Function Problems
  TOOL_FUNCTIONS TOOLS;

  private:

  void clean_chat_conversation();
    
  nlohmann::json build_generate_request(string Request);

  nlohmann::json build_chat_request(string Role_1, string Name_1, string Content_1, 
                                string Role_2, string Name_2, string Content_2, 
                                bool Enable_Tool_Function, nlohmann::json Message_Append);

  void submit_generate_request(string Request, 
    bool Output_To_Response, bool Consider_Context, 
    bool Remember_Context);

  void submit_chat_request(string Role_1, string Name_1, string Question_1, 
    string Role_2, string Name_2, string Question_2, 
    bool Output_To_Response, bool Consider_Context, 
    bool Remember_Context, bool Enable_Tool_Function, 
    nlohmann::json Message_Append);

  void create();
  // Generates connection between Ollama server and Ollama API.

  //void exec_question();
  // Starts new thread with parameters set in mutex

  public:

  void set_status(int Status);
  // force sets OLLAMA_API_STATUS as defined in header
  //  should only be used when bringing Ollama_API_STATUS to DONE after
  //  OLLAMA_API_READY_TO_GATHER.

  int get_status();
  // Returns OLLAMA_API_STATUS as defined in header

  void check();
  // Check to see if Ollama server is connected then creates the api session if not. 

  void submit_request(string Role_1, string Name_1, string Question_1, 
    string Role_2, string Name_2, string Question_2, 
    bool Output_To_Response, bool Consider_Context, 
    bool Remember_Context, bool Enable_Tool_Function, 
    nlohmann::json Message_Append);
  void submit_request(string Role_1, string Name_1, string Question_1, 
    string Role_2, string Name_2, string Question_2, 
    bool Output_To_Response, bool Consider_Context, 
    bool Remember_Context, bool Enable_Tool_Function);
  void submit_request(string Role, string Name, string Question, 
    bool Output_To_Response, bool Consider_Context, 
    bool Remember_Context, bool Enable_Tool_Function);
  // Question - Input into ollama ai
  // Role - system assistant or user
  // Name - user name or for user role
  // Output_To_Response - if true, output is generated to output field.
  // Consider_Context - if true, consider the previous context when generating response.
  // Remember_Context - if true, generated response is remembered for future use.
  //                    if Consider_Context is false, generated context will be combined  
  //                    with previous context.

  int check_response();
  // Routine to get a currently running ollama response and update 
  //  the accessible live response stream

  void check_response_done(THOUGHT_VARIABLES &Thought_Control);
  // Routine called by process to close and set things when Ollama is 
  //  finish responding

  string get_complete_text_response();
  // Returns complete text response at the time of status done.

  // Push current context to stack.  remember to match unpause with pause.
  //  or context will not be restored
  void context_pause();
  void context_unpause();

  void process(TTY_OUTPUT &Output, TTY_OUTPUT_FOCUS &Focus, THOUGHT_VARIABLES &Thought_Control);
  // If Ollama server is active and responding, outputs genereted text.
  // If Ollama has completed genereated text, post processing is called.
  // else, routine does nothing.
};

// ------------------------------------------------------------------------- //

#endif // OLLAMA_API_H

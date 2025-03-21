#ifndef OLLAMA_API_H
#define OLLAMA_API_H

#include <iostream>
#include <string>
#include <vector>

#include "ollama.hpp"   // Include this for: Fatal glibc error: pthread_mutex_lock.c:94 (___pthread_mutex_lock): assertion failed: mutex->__data.__owner == 0

#include "helper.h"
#include "tty_io.h"
#include "threading.h"

#include "rasapi.h"

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
#define USER_INSTRUCTIONS "INSTRUCTIONS"

using namespace std;

// ------------------------------------------------------------------------- //

class OLLAMA_API_MUTEX
{
  private:

  mutable mutex MUTEX_;

  vector<string> RESPONSE_VECTOR;
  int DONE = 0;

  ollama::response COMPLETE_RESPONSE_AFTER_DONE;
  bool COMPLETE_RESPONSE_READY_AFTER_DONE = false;

  bool CHANGED = false;

  public:

  // Getting a copy of the response afte it is done.
  void set_complete_response_after_done(ollama::response Response);

  ollama::response get_complete_response_after_done();
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

class MESSAGE
{
  public:
  string role;
  string content;
};

// ------------------------------------------------------------------------- //

class OLLAMA_API_PROPS
{
  public:

  string MODEL = "";  // "llama3.1:8b"
  string URL   = "";  // "http://localhost:11434"
};

class OLLAMA_API
{
  // ------------------------------------------------------------------------- //
  // This section reserved for async
  private:
  
  OLLAMA_API_MUTEX OLLAMA_MUTEX;

  int CONNECTION_STATUS = 0;
  bool CONNECTION_STATUS_CHANGED = true;

  std::function<void(const ollama::response&)> response_callback;
  void on_receive_response(const ollama::response& Request);

  public:

  void proc_render_thread();
  // ------------------------------------------------------------------------- //

  private:
  Ollama OLLAMA;

  vector<string> OLLAMA_MODELS_LIST;
  vector<string> OLLAMA_MODELS_RUNNING;

  ollama::options OPTIONS;

  ollama::response RESPONSE;

  // CHAT MODEL
  ollama::messages CONVERSATION;

  ollama::response CONTEXT;
  vector<ollama::response> CONTEXT_PAUSED;

  // Different types of ask.
  bool REMEMBER_CONTEXT = true;
  bool CONSIDER_CONTEXT = true;
  bool ALLOW_OUTPUT     = true;

  public:

  OLLAMA_API_PROPS PROPS;

  THREADING_INFO  OLLAMA_RESPONSE_THREAD;

  vector<string> RESPONSE_STRING_VECTOR;
  string RESPONSE_FULL = "";  // RESPONSE.as_simple_string(); not working so
                              //  as fall back

  // Sending
  string REQUEST = "";

  private:
  
  std::vector<MESSAGE> conversation;

  nlohmann::json build_request(string Role, string Name, string Content);
  nlohmann::json build_request(string Role1, string Name1, string Content1, string Role2, string Name2, string Content2);

  void create();
  // Generates connection between Ollama server and Ollama API.

  void exec_question();
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

  void submit_question(string Role, string Name, string Question, bool Output_To_Response, bool Consider_Context, bool Remember_Context);
  // Question - Input into ollama ai
  // Role - system assistant or user
  // Name - user name or for user role
  // Output_To_Response - if true, output is generated to output field.
  // Consider_Context - if true, consider the previous context when generating response.
  // Remember_Context - if true, generated response is remembered for future use.
  //                    if Consider_Context is false, generated context will be combined  
  //                    with previous context.
  void submit_question(string Role_1, string Name_1, string Question_1, 
                        string Role_2, string Name_2, string Question_2, 
                        bool Output_To_Response, bool Consider_Context, bool Remember_Context);

  int check_response();
  // Routine to get a currently running ollama response and update 
  //  the accessible live response stream

  void check_response_done();
  // Routine called by process to close and set things when Ollama is 
  //  finish responding

  string get_complete_text_response();
  // Returns complete text response at the time of status done.

  // Push current context to stack.  remember to match unpause with pause.
  //  or context will not be restored
  void context_pause();
  void context_unpause();

  void process(TTY_OUTPUT &Output, TTY_OUTPUT_FOCUS &Focus);
  // If Ollama server is active and responding, outputs genereted text.
  // If Ollama has completed genereated text, post processing is called.
  // else, routine does nothing.
};

// ------------------------------------------------------------------------- //

#endif // OLLAMA_API_H

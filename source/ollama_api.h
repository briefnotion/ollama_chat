#ifndef OLLAMA_API_H
#define OLLAMA_API_H

#include <iostream>
#include <string>
#include <vector>

#include "ollama.hpp"   // Include this for: Fatal glibc error: pthread_mutex_lock.c:94 (___pthread_mutex_lock): assertion failed: mutex->__data.__owner == 0

#include "helper.h"
#include "tty_io.h"
#include "threading.h"

#define OLLAMA_RESPONSE_THREAD_TIMER_DELAY   60   // This will be in frames per second (fps)

#define OLLAMA_API_READY_FOR_REQUEST      0
//#define OLLAMA_API_WRITING_REQUEST        1
//#define OLLAMA_API_REQUEST_SUBMITTED      2 
#define OLLAMA_API_RESPONS_GENERATING     3
#define OLLAMA_API_RESPONSE_DONE          4

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
  bool complete_respoonse_ready_after_done() const;

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
  ollama::response CONTEXT;

  public:

  OLLAMA_API_PROPS PROPS;

  THREADING_INFO  OLLAMA_RESPONSE_THREAD;

  vector<string> RESPONSE_STRING_VECTOR;

  // Sending
  string REQUEST = "";

  bool create(TTY_OUTPUT &Output_Container, TTY_OUTPUT_FOCUS &Output_Focus);

  void exec_question();

  int get_status();

  void set_status(int Status);

  void submit_question(const string& Question);

  int check_response();

  void check_response_done();

  void process(TTY_OUTPUT &Output, TTY_OUTPUT_FOCUS &Focus);
};

// ------------------------------------------------------------------------- //

#endif // OLLAMA_API_H

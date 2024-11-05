#ifndef OLLAMA_API_H
#define OLLAMA_API_H

#include <iostream>
#include <string>
#include <vector>

#include "ollama.hpp"   // Include this for: Fatal glibc error: pthread_mutex_lock.c:94 (___pthread_mutex_lock): assertion failed: mutex->__data.__owner == 0

#include "helper.h"
#include "ollama_helper.h"
#include "tty_io.h"

using namespace std;

// ------------------------------------------------------------------------- //

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

  vector<string> RESPONSE_STRING_VECTOR;

  // Sending
  string REQUEST = "";

  bool create(TTY_OUTPUT &Output_Container, TTY_OUTPUT_FOCUS &Output_Focus);

  int get_status();

  void set_status(int Status);

  void set_request(const string& request);

  int check_response();

  void check_response_done();
};

// ------------------------------------------------------------------------- //

#endif // OLLAMA_API_H

#ifndef OLLAMA_API_H
#define OLLAMA_API_H

//#include <string>
//#include <vector>

/*
  ollama-hpp is a header-only C++ implementation of a state-of-the-art AI model.
  This library was created by Jonathan Monty and is available under the MIT License.
  For more details visit: https://github.com/jmont-dev/ollama-hpp
*/
#include "ollama.hpp"

#include "helper.h"
#include "ollama_helper.h"

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

  bool create();

  int get_status();

  void set_ready_for_request();

  void set_questioning();

  void set_request(const string& request);

  int check_response();

  void check_response_done();
};

// ------------------------------------------------------------------------- //

#endif // OLLAMA_API_H

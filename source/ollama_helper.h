#ifndef OLLAMA_HELPER_H
#define OLLAMA_HELPER_H

#include <iostream>
#include <string>
#include <vector>
#include <mutex>

#include <nlohmann/json.hpp>
#include "ollama.hpp"

#define OLLAMA_API_READY_FOR_REQUEST      0
#define OLLAMA_API_WRITING_REQUEST        1
#define OLLAMA_API_REQUEST_SUBMITTED      2 
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

#endif  // OLLAMA_HELPER_H

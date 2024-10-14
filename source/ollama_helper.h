#ifndef OLLAMA_HELPER_H
#define OLLAMA_HELPER_H

#include <iostream>
#include <string>
#include <vector>
#include <mutex>

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

  string REQUEST = "";
  vector<string> RESPONSE_VECTOR;
  int DONE = 0;

  public:

  int done() const;
  void set_done(int Done);  

  void set_request(string Request);
  string request() const;
  void add_to_response(const string& value);
  int response_size() const;
  void get_response_to_vector(vector<string> &Receiving_Vector);
};

// ------------------------------------------------------------------------- //

#endif  // OLLAMA_HELPER_H

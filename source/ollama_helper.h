#ifndef OLLAMA_HELPER_H
#define OLLAMA_HELPER_H

#include <string>
#include <vector>
#include <mutex>

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
  // 0 = call question
  // 1= writing question
  // 2 = quistion waiting, 
  // 3 = respons generating
  // 4 = response done.

  void set_request(string Request);
  string request() const;
  void add_to_response(const string& value);
  int response_size() const;
  string return_vector_as_string();
};

// ------------------------------------------------------------------------- //


#endif  // OLLAMA_HELPER_H

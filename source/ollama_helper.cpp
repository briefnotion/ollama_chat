#ifndef OLLAMA_HELPER_CPP
#define OLLAMA_HELPER_CPP

#include "ollama_helper.h"

using namespace std;

// ------------------------------------------------------------------------- //

int OLLAMA_API_MUTEX::done() const
{
  lock_guard<mutex> lock(MUTEX_);
  return DONE;
}

void OLLAMA_API_MUTEX::set_done(int Done)
{
  lock_guard<mutex> lock(MUTEX_);
  DONE = Done;
}

void OLLAMA_API_MUTEX::set_request(string Request)
{
  lock_guard<mutex> lock(MUTEX_);
  REQUEST = Request;
}

string OLLAMA_API_MUTEX::request() const
{
  lock_guard<mutex> lock(MUTEX_);
  return REQUEST;
}

void OLLAMA_API_MUTEX::add_to_response(const string& value) 
{
  lock_guard<mutex> lock(MUTEX_);
  RESPONSE_VECTOR.push_back(value);
}

int OLLAMA_API_MUTEX::response_size() const
{
  lock_guard<mutex> lock(MUTEX_);
  return RESPONSE_VECTOR.size();
}

string OLLAMA_API_MUTEX::return_vector_as_string()
{
  lock_guard<mutex> lock(MUTEX_);
  string ret_string = "";

  if (RESPONSE_VECTOR.size() > 0)
  {
    for (int i = 0; i < (int)RESPONSE_VECTOR.size(); ++i) 
    {
      ret_string += RESPONSE_VECTOR[i];
    }
  }

  RESPONSE_VECTOR.clear();
  return ret_string;
}

// ------------------------------------------------------------------------- //

#endif  // OLLAMA_HELPER_CPP
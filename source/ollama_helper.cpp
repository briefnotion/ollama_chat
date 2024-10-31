#ifndef OLLAMA_HELPER_CPP
#define OLLAMA_HELPER_CPP

#include "ollama_helper.h"

using namespace std;

// ------------------------------------------------------------------------- //

/*

void OLLAMA_API_MUTEX::set_complete_response_after_done(ollama::response Response)
{
  lock_guard<mutex> lock(MUTEX_);

  COMPLETE_RESPONSE_AFTER_DONE = Response;
  COMPLETE_RESPONSE_READY_AFTER_DONE = true;
}

ollama::response OLLAMA_API_MUTEX::get_complete_response_after_done()
{
  lock_guard<mutex> lock(MUTEX_);

  ollama::response ret_response = COMPLETE_RESPONSE_AFTER_DONE;

  // Clear the response.
  COMPLETE_RESPONSE_AFTER_DONE = ollama::response();
  COMPLETE_RESPONSE_READY_AFTER_DONE = false;

  CHANGED = false;

  return ret_response;
}

bool OLLAMA_API_MUTEX::complete_respoonse_ready_after_done() const
{
  lock_guard<mutex> lock(MUTEX_);

  if (CHANGED)
  {
    return COMPLETE_RESPONSE_READY_AFTER_DONE;
  }
  else
  {
    return false;
  }
}

int OLLAMA_API_MUTEX::done() const
{
  lock_guard<mutex> lock(MUTEX_);
  return DONE;
}

void OLLAMA_API_MUTEX::set_done(int Done)
{
  lock_guard<mutex> lock(MUTEX_);
  DONE = Done;

  if (DONE == OLLAMA_API_RESPONSE_DONE)
  {
    CHANGED = true;
  }
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

void OLLAMA_API_MUTEX::get_response_to_vector(vector<string> &Receiving_Vector)
{
  lock_guard<mutex> lock(MUTEX_);

  if (RESPONSE_VECTOR.size() > 0)
  {
    for (int i = 0; i < (int)RESPONSE_VECTOR.size(); ++i) 
    {
      Receiving_Vector.push_back(RESPONSE_VECTOR[i]);
    }
  }
  
  RESPONSE_VECTOR.clear();
}

// ------------------------------------------------------------------------- //
*/
#endif  // OLLAMA_HELPER_CPP
#ifndef VECTORDB_PYTHON_API_CPP
#define VECTORDB_PYTHON_API_CPP


#include "vectordb_python_api.h"

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

bool OLLAMA_API_MUTEX::complete_response_ready_after_done() const
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
*/

void VECTORDB_PYTHON_MUTEX::set_command_line(string Command)
{
  lock_guard<mutex> lock(MUTEX_);
  COMMAND_LINE = Command;
}

string VECTORDB_PYTHON_MUTEX::command_line() const
{
  lock_guard<mutex> lock(MUTEX_);
  return COMMAND_LINE;
}

int VECTORDB_PYTHON_MUTEX::done() const
{
  lock_guard<mutex> lock(MUTEX_);
  return DONE;
}

void VECTORDB_PYTHON_MUTEX::set_done(int Done)
{
  lock_guard<mutex> lock(MUTEX_);
  DONE = Done;

  if (DONE == OLLAMA_API_RESPONSE_DONE)
  {
    CHANGED = true;
  }
}

void VECTORDB_PYTHON_MUTEX::add_to_response(const string& value) 
{
  lock_guard<mutex> lock(MUTEX_);
  RESPONSE_VECTOR.push_back(value);
}

int VECTORDB_PYTHON_MUTEX::response_size() const
{
  lock_guard<mutex> lock(MUTEX_);
  return RESPONSE_VECTOR.size();
}

void VECTORDB_PYTHON_MUTEX::get_response_to_vector(vector<string> &Receiving_Vector)
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

string VECTORDB_PYTHON_API::exec(const char* cmd) 
{
  array<char, 128> buffer;
  string result;
  unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
  if (!pipe) 
  {
    throw runtime_error("popen() failed!");
  }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) 
  {
    result += buffer.data();
  }
  return result;
}

void VECTORDB_PYTHON_API::exec_thread_question() 
{
  array<char, 128> buffer;
  unique_ptr<FILE, decltype(&pclose)> pipe(popen(PYTHON_QUESTION_RESPONSE_MUTEX.command_line().c_str(), "r"), pclose);

  PYTHON_QUESTION_RESPONSE_MUTEX.set_done(1);

  if (!pipe) 
  {
    PYTHON_QUESTION_RESPONSE_MUTEX.set_done(0);
    throw runtime_error("popen() failed!");
  }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) 
  {
    PYTHON_QUESTION_RESPONSE_MUTEX.add_to_response(buffer.data());
  }

  PYTHON_QUESTION_RESPONSE_MUTEX.set_done(2);
}

int VECTORDB_PYTHON_API::get_status()
{
  return PYTHON_QUESTION_RESPONSE_MUTEX.done();
}

void VECTORDB_PYTHON_API::submit_question(double Time, string Question) 
{
  const string andand = "&& ";

  string bcommand = PROPS.ENVIRONMENT + andand + PROPS.SCRIPT_SEARCH + Question;

  if (PROPS.BASH_SHELL.size() > 0)
  {
    bcommand = PROPS.BASH_SHELL + bcommand + "'";
  }

  PYTHON_QUESTION_RESPONSE_MUTEX.set_command_line (bcommand);

  if (PYTHON_QUESTION_RESPONSE_MUTEX.done() == 0)
  {
    //exec_thread_question();

    // Be careful with this because it looks like black magic to me.
    PYTHON_QUESTION_RESPONSE_THREAD.start_render_thread([&]() 
                  {  exec_thread_question();  });
  }
}

void VECTORDB_PYTHON_API::submit_file_to_embed(string File)
{
  const string andand = "&& ";

  string bcommand = PROPS.ENVIRONMENT + andand + PROPS.SCRIPT_EMBED_FILE + File;

  if (PROPS.BASH_SHELL.size() > 0)
  {
    bcommand = PROPS.BASH_SHELL + bcommand + "'";
  }

  PYTHON_QUESTION_RESPONSE_MUTEX.set_command_line (bcommand);

  if (PYTHON_QUESTION_RESPONSE_MUTEX.done() == 0)
  {
    //exec_thread_question();

    // Be careful with this because it looks like black magic to me.
    PYTHON_QUESTION_RESPONSE_THREAD.start_render_thread([&]() 
                  {  exec_thread_question();  });
  }
}

string VECTORDB_PYTHON_API::process()
{
  string ret_response = "";
  if (PYTHON_QUESTION_RESPONSE_MUTEX.response_size() > 0)
  {
    vector<string> result;
    PYTHON_QUESTION_RESPONSE_MUTEX.get_response_to_vector(result);
    ret_response = return_vector_as_string(result);
  }

  // Post Process
  if (PYTHON_QUESTION_RESPONSE_MUTEX.done() == 2)
  {
    // Post Process Here
    PYTHON_QUESTION_RESPONSE_MUTEX.set_done(0);
  }

  // change to addto
  return ret_response;
}



// ------------------------------------------------------------------------- //

#endif // VECTORDB_PYTHON_API_CPP
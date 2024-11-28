#ifndef VECTORDB_PYTHON_API_CPP
#define VECTORDB_PYTHON_API_CPP


#include "vectordb_python_api.h"

using namespace std;

// ------------------------------------------------------------------------- //

string VECTORDB_PYTHON_MUTEX::get_complete_response() const
{
  lock_guard<mutex> lock(MUTEX_);

  return COMPLETE_RESPONSE;
}

void VECTORDB_PYTHON_MUTEX::set_command_line(string Command)
{
  lock_guard<mutex> lock(MUTEX_);
  COMPLETE_RESPONSE = "";
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

  if (DONE == VECTORDB_API_RESPONSE_DONE)
  {
    CHANGED = true;
  }
}

void VECTORDB_PYTHON_MUTEX::add_to_response(const string& value) 
{
  lock_guard<mutex> lock(MUTEX_);
  RESPONSE_VECTOR.push_back(value);
  COMPLETE_RESPONSE += value;
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

  PYTHON_QUESTION_RESPONSE_MUTEX.set_done(VECTORDB_API_RESPONS_GENERATING);

  if (!pipe) 
  {
    PYTHON_QUESTION_RESPONSE_MUTEX.set_done(VECTORDB_API_READY_FOR_REQUEST);
    throw runtime_error("popen() failed!");
  }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) 
  {
    PYTHON_QUESTION_RESPONSE_MUTEX.add_to_response(buffer.data());
  }

  PYTHON_QUESTION_RESPONSE_MUTEX.set_done(VECTORDB_API_RESPONSE_DONE);
}

void VECTORDB_PYTHON_API::thread()
{
  if (PYTHON_QUESTION_RESPONSE_MUTEX.done() == VECTORDB_API_READY_FOR_REQUEST)
  {
    //exec_thread_question();

    // Be careful with this because it looks like black magic to me.
    PYTHON_QUESTION_RESPONSE_THREAD.start_render_thread([&]() 
                  {  exec_thread_question();  });
  }
}

int VECTORDB_PYTHON_API::get_status()
{
  return PYTHON_QUESTION_RESPONSE_MUTEX.done();
}

void VECTORDB_PYTHON_API::submit_question(string Question) 
{
  QUESTION = Question;
  DOCS_ONLY = false;

  const string andand = "&& ";

  string bcommand = PROPS.ENVIRONMENT + andand + PROPS.SCRIPT_SEARCH + QUESTION;

  if (PROPS.BASH_SHELL.size() > 0)
  {
    bcommand = PROPS.BASH_SHELL + bcommand + "'";
  }

  PYTHON_QUESTION_RESPONSE_MUTEX.set_command_line (bcommand);
  thread();
}

void VECTORDB_PYTHON_API::submit_question_to_ollama(string Question) 
{
  QUESTION = Question;
  DOCS_ONLY = true;

  const string andand = "&& ";

  string bcommand = PROPS.ENVIRONMENT + andand + PROPS.SCRIPT_SEARCH_DOCS_ONLY + QUESTION;

  if (PROPS.BASH_SHELL.size() > 0)
  {
    bcommand = PROPS.BASH_SHELL + bcommand + "'";
  }

  PYTHON_QUESTION_RESPONSE_MUTEX.set_command_line (bcommand);
  thread();
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
  thread();
}

void VECTORDB_PYTHON_API::submit_clear_database()
{
  const string andand = "&& ";

  string bcommand = PROPS.ENVIRONMENT + andand + PROPS.SCRIPT_CLEAR_DATABASE;

  if (PROPS.BASH_SHELL.size() > 0)
  {
    bcommand = PROPS.BASH_SHELL + bcommand + "'";
  }

  PYTHON_QUESTION_RESPONSE_MUTEX.set_command_line (bcommand);
  thread();
}

void VECTORDB_PYTHON_API::submit_list_database()
{
  const string andand = "&& ";

  string bcommand = PROPS.ENVIRONMENT + andand + PROPS.SCRIPT_LIST_DATABASE;

  if (PROPS.BASH_SHELL.size() > 0)
  {
    bcommand = PROPS.BASH_SHELL + bcommand + "'";
  }

  PYTHON_QUESTION_RESPONSE_MUTEX.set_command_line (bcommand);
  thread();
}

void VECTORDB_PYTHON_API::process(TTY_OUTPUT &Output, TTY_OUTPUT_FOCUS &Focus, OLLAMA_API &Ollama_System)
{
  if (DOCS_ONLY)
  {
    if (PYTHON_QUESTION_RESPONSE_MUTEX.done() == VECTORDB_API_RESPONS_GENERATING)
    {
      //Output.add_to("generating docs", Focus);
    }
    else if (PYTHON_QUESTION_RESPONSE_MUTEX.done() == VECTORDB_API_RESPONSE_DONE)
    {
      //Output.add_to("\n\n-----\n", Focus);
      // Post Process Here

      string question_with_docs = QUESTION + " - Answer that question using the following text as a resource: " + PYTHON_QUESTION_RESPONSE_MUTEX.get_complete_response();

      //Output.add_to(question_with_docs, Focus);
      Ollama_System.submit_question(question_with_docs);

      QUESTION = "";
      PYTHON_QUESTION_RESPONSE_MUTEX.set_done(VECTORDB_API_READY_FOR_REQUEST);
    }
  }
  else
  {
    string ret_response = "";

    if (PYTHON_QUESTION_RESPONSE_MUTEX.response_size() > 0)
    {
      vector<string> result;
      PYTHON_QUESTION_RESPONSE_MUTEX.get_response_to_vector(result);
      ret_response = return_vector_as_string(result);
    }

    Output.add_to(ret_response, Focus);

    // Post Process
    if (PYTHON_QUESTION_RESPONSE_MUTEX.done() == VECTORDB_API_RESPONSE_DONE)
    {
      Output.add_to("\n\n-----\n", Focus);
      
      // Post Process Here
      QUESTION = "";
      PYTHON_QUESTION_RESPONSE_MUTEX.set_done(VECTORDB_API_READY_FOR_REQUEST);
    }
  }
}



// ------------------------------------------------------------------------- //

#endif // VECTORDB_PYTHON_API_CPP
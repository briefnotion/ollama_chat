#ifndef VECTORDB_PYTHON_API_H
#define VECTORDB_PYTHON_API_H

#include <iostream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>

#include "ollama_api.h"
#include "threading.h"

#define  VECTORDB_API_RESPONSE_THREAD_TIMER_DELAY   60   // This will be in frames per second (fps)

#define VECTORDB_API_READY_FOR_REQUEST      0
//#define VECTORDB_API_WRITING_REQUEST        1
//#define VECTORDB_API_REQUEST_SUBMITTED      2 
#define VECTORDB_API_RESPONS_GENERATING     3
#define VECTORDB_API_RESPONSE_DONE          4

using namespace std;

// ------------------------------------------------------------------------- //
class VECTORDB_PYTHON_MUTEX
{
  private:

  mutable mutex MUTEX_;

  vector<string> RESPONSE_VECTOR;
  int DONE = VECTORDB_API_READY_FOR_REQUEST;

  string COMMAND_LINE = "";

  int TEXT_RESPONSE_SIZE = 0;

  bool CHANGED = false;

  public:

  // Submitting Question
  void set_command_line(string Command);
  string command_line() const;

  // Getting a copy of the response afte it is done.
  //void set_complete_response_after_done(ollama::response Response);

  //ollama::response get_complete_response_after_done();
  //bool complete_respoonse_ready_after_done() const;

  // Generating Response Complete
  int done() const;
  void set_done(int Done);  

  // Text Response Generating
  void add_to_response(const string& value);
  int response_size() const;
  void get_response_to_vector(vector<string> &Receiving_Vector);
};

class VECTORDB_PYTHON_API_PROPERTIES
{
  public:

  // Command Line Input
  string BASH_SHELL =             "bash -c '";
  string ENVIRONMENT =            "source /home/briefn/py/venv/bin/activate ";
  string SCRIPT_SEARCH =          "python3 ../python/search.py ";
  string SCRIPT_EMBED_FILE =      "python3 ../python/import_sp.py ";
  string SCRIPT_CLEAR_DATABASE =  "python3 ../python/clear_database.py";
  string SCRIPT_LIST_DATABASE =  "python3 ../python/list_database.py";
};

class VECTORDB_PYTHON_API
{
  private:

  VECTORDB_PYTHON_MUTEX PYTHON_QUESTION_RESPONSE_MUTEX;

  string exec(const char* cmd);
  void exec_thread_question();
  void thread();

  public:

  VECTORDB_PYTHON_API_PROPERTIES PROPS;

  THREADING_INFO  PYTHON_QUESTION_RESPONSE_THREAD;

  int get_status();

  void submit_question(string Question);
  void submit_file_to_embed(string File);
  void submit_clear_database();
  void submit_list_database();

  void process(TTY_OUTPUT &Output, TTY_OUTPUT_FOCUS &Focus);

};

// ------------------------------------------------------------------------- //

#endif // VECTORDB_PYTHON_API_H
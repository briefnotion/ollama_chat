#ifndef VECTORDB_PYTHON_API_H
#define VECTORDB_PYTHON_API_H

#include <iostream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>

#include "ollama_api.h"
#include "threading.h"
#include "stringthings.h"

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
  string COMPLETE_RESPONSE = "";

  int DONE = VECTORDB_API_READY_FOR_REQUEST;

  string COMMAND_LINE = "";

  int TEXT_RESPONSE_SIZE = 0;

  bool CHANGED = false;

  public:

  // Submitting Question
  void set_command_line(string Command);
  string command_line() const;

  // Getting a copy of the response afte it is done.
  string get_complete_response() const;

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
  string BASH_SHELL =               "bash -c '";
  string ENVIRONMENT =              "source /home/briefn/py/venv/bin/activate ";
  string SCRIPT_SEARCH =            "python3 ../python/search.py ";
  string SCRIPT_SEARCH_DOCS_ONLY =  "python3 ../python/search_docs_only.py ";
  string SCRIPT_EMBED_FILE =        "python3 ../python/import_sp.py ";
  string SCRIPT_CLEAR_DATABASE =    "python3 ../python/clear_database.py";
  string SCRIPT_LIST_DATABASE =     "python3 ../python/list_database.py";
};

class VECTORDB_PYTHON_API
{
  private:

  VECTORDB_PYTHON_MUTEX PYTHON_QUESTION_RESPONSE_MUTEX;

  string exec(const char* cmd);
  void exec_thread_question();
  void thread();

  string QUESTION = "";

  public:

  VECTORDB_PYTHON_API_PROPERTIES PROPS;

  THREADING_INFO  PYTHON_QUESTION_RESPONSE_THREAD;

  int DOCS_ONLY = false;
  string APP_TYPE = "";

  int get_status();

  void submit_question(string Question);
  void submit_question_to_ollama(string Question, string App_Type);
  void submit_question_to_ollama_par(string Question, string App_Type, OLLAMA_API &Ollama_System);

  void submit_file_to_embed(string File);
  void submit_clear_database();
  void submit_list_database();

  void process(TTY_OUTPUT &Output, TTY_OUTPUT_FOCUS &Focus, OLLAMA_API &Ollama_System);

};

// ------------------------------------------------------------------------- //

#endif // VECTORDB_PYTHON_API_H
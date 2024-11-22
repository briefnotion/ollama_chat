#ifndef VECTORDB_PYTHON_API_H
#define VECTORDB_PYTHON_API_H

#include <iostream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>

#include "ollama_api.h"
#include "threading.h"

using namespace std;

// ------------------------------------------------------------------------- //
class VECTORDB_PYTHON_MUTEX
{
  private:

  mutable mutex MUTEX_;

  vector<string> RESPONSE_VECTOR;
  int DONE = 0;
                  // 0 - not doing anything
                  // 1 - getting data
                  // 2 - data complete, needs grabbing.

  //ollama::response COMPLETE_RESPONSE_AFTER_DONE;
  //bool COMPLETE_RESPONSE_READY_AFTER_DONE = false;

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
  string BASH_SHELL = "bash -c '";
  string ENVIRONMENT = "source /home/briefn/py/venv/bin/activate ";
  string SCRIPT_SEARCH = "python3 ../python/search.py ";


};

class VECTORDB_PYTHON_API
{
  private:

  VECTORDB_PYTHON_MUTEX PYTHON_QUESTION_RESPONSE_MUTEX;

  string exec(const char* cmd);

  void exec_thread_question();

  public:

  VECTORDB_PYTHON_API_PROPERTIES PROPS;

  THREADING_INFO  PYTHON_QUESTION_RESPONSE_THREAD;

  int get_status();

  void submit_question(unsigned long Time, string Question);

  string process();

};

// ------------------------------------------------------------------------- //

#endif // VECTORDB_PYTHON_API_H
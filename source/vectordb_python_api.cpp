#ifndef VECTORDB_PYTHON_API_CPP
#define VECTORDB_PYTHON_API_CPP


#include "vectordb_python_api.h"

using namespace std;

// ------------------------------------------------------------------------- //

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

string VECTORDB_PYTHON_MUTEX::get_complete_response() const
{
  lock_guard<mutex> lock(MUTEX_);

  return COMPLETE_RESPONSE;
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
  
  // Use a lambda to avoid the warning
  unique_ptr<FILE, function<int(FILE*)>> pipe(popen(cmd, "r"), [](FILE* fp) { return pclose(fp); });
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

  // Use a lambda to avoid the warning
  unique_ptr<FILE, function<int(FILE*)>> pipe(popen(PYTHON_QUESTION_RESPONSE_MUTEX.command_line().c_str(), "r"), [](FILE* fp) { return pclose(fp); });

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
  if (PYTHON_QUESTION_RESPONSE_MUTEX.done() == VECTORDB_API_READY_FOR_REQUEST || 
      PYTHON_QUESTION_RESPONSE_MUTEX.done() == VECTORDB_API_WRITING_REQUEST)
  {
    //exec_thread_question();

    // Be careful with this because it looks like black magic to me.
    PYTHON_QUESTION_RESPONSE_THREAD.start_render_thread([&]() 
                  {  exec_thread_question();  });
  }
}

void VECTORDB_PYTHON_API::set_status(int Status)
{
  PYTHON_QUESTION_RESPONSE_MUTEX.set_done(Status);
}

int VECTORDB_PYTHON_API::get_status()
{
  return PYTHON_QUESTION_RESPONSE_MUTEX.done();
}

void VECTORDB_PYTHON_API::clear_thoughts()
{
  THINKING = false; 
  ABOUT = "";
  THINKING_STAGE = -1;
  SUBJECT = "";
  COLLECTION_NAME = "";

  GATHERED_DOCUMENTS_RELEVANT = false;
  GATHERED_DOCUMENTS = "";

  set_status(VECTORDB_API_READY_FOR_REQUEST);
}

void VECTORDB_PYTHON_API::process_gathering_information_stages()
{
  if (THINKING_STAGE == 0)
  {
    // Submit call to search docs
    if (get_status() == VECTORDB_API_READY_FOR_REQUEST)
    {
      string bcommand = PROPS.ENVIRONMENT + aa + PROPS.SCRIPT_SEARCH_DOCS_ONLY_COS + sp + COLLECTION_NAME + sp + SUBJECT;

      if (PROPS.BASH_SHELL.size() > 0)
      {
        bcommand = PROPS.BASH_SHELL + bcommand + "'";
      }

      PYTHON_QUESTION_RESPONSE_MUTEX.set_command_line (bcommand);
      thread();

      THINKING_STAGE = 1;
    }
  }
  else if (THINKING_STAGE == 1)
  {
    // Wait for response
    if (get_status() == VECTORDB_API_RESPONSE_DONE)
    {
      // Determine if anything is relevant
      bool relevant_documents = false;

      if (PYTHON_QUESTION_RESPONSE_MUTEX.get_complete_response().size() >= 22)
      {
        if (PYTHON_QUESTION_RESPONSE_MUTEX.get_complete_response().substr(0, 22) != "!No Relevant Documents")
        {
          relevant_documents = true;
        }
      }

      if (relevant_documents)
      {
        // If Documents are Relevant, put them in a container. Signify Completion.
        GATHERED_DOCUMENTS_RELEVANT = true;
        GATHERED_DOCUMENTS = PYTHON_QUESTION_RESPONSE_MUTEX.get_complete_response();
      }
      else
      {
        // If No Documents are Relevant. Signify Completion.
        GATHERED_DOCUMENTS_RELEVANT = false;
        GATHERED_DOCUMENTS = "";
      }

      set_status(VECTORDB_API_RESPONSE_READY_TO_GATHER);
    }
  }
}

void VECTORDB_PYTHON_API::process_list_database_stages()
{
  if (THINKING_STAGE == 0)
  {
    // Submit call to search docs
    if (get_status() == VECTORDB_API_READY_FOR_REQUEST)
    {
      string bcommand = PROPS.ENVIRONMENT + aa + PROPS.SCRIPT_LIST_DATABASE;

      if (PROPS.BASH_SHELL.size() > 0)
      {
        bcommand = PROPS.BASH_SHELL + bcommand + "'";
      }

      PYTHON_QUESTION_RESPONSE_MUTEX.set_command_line (bcommand);
      thread();

      THINKING_STAGE = 1;
    }
  }
  else if (THINKING_STAGE == 1)
  {
    // Wait for response
    if (get_status() == VECTORDB_API_RESPONSE_DONE)
    {
      GATHERED_DOCUMENTS = PYTHON_QUESTION_RESPONSE_MUTEX.get_complete_response();
      set_status(VECTORDB_API_RESPONSE_READY_TO_GATHER);
    }
  }
}

void VECTORDB_PYTHON_API::process_embed_documents_stages()
{
  if (THINKING_STAGE == 0)
  {
    // Submit call to search docs
    if (get_status() == VECTORDB_API_READY_FOR_REQUEST)
    {
      string bcommand = PROPS.ENVIRONMENT + aa + PROPS.SCRIPT_EMBED_FILE + sp + COLLECTION_NAME + sp + SUBJECT;

      if (PROPS.BASH_SHELL.size() > 0)
      {
        bcommand = PROPS.BASH_SHELL + bcommand + "'";
      }

      PYTHON_QUESTION_RESPONSE_MUTEX.set_command_line (bcommand);
      thread();

      THINKING_STAGE = 1;
    }
  }
  else if (THINKING_STAGE == 1)
  {
    // Wait for response
    if (get_status() == VECTORDB_API_RESPONSE_DONE)
    {
      GATHERED_DOCUMENTS = PYTHON_QUESTION_RESPONSE_MUTEX.get_complete_response();
      set_status(VECTORDB_API_RESPONSE_READY_TO_GATHER);
    }
  }
}

void VECTORDB_PYTHON_API::process_clear_database_stages()
{
  if (THINKING_STAGE == 0)
  {
    // Submit call to search docs
    if (get_status() == VECTORDB_API_READY_FOR_REQUEST)
    {
      string bcommand = PROPS.ENVIRONMENT + aa + PROPS.SCRIPT_CLEAR_DATABASE;

      if (PROPS.BASH_SHELL.size() > 0)
      {
        bcommand = PROPS.BASH_SHELL + bcommand + "'";
      }

      PYTHON_QUESTION_RESPONSE_MUTEX.set_command_line (bcommand);
      thread();

      THINKING_STAGE = 1;
    }
  }
  else if (THINKING_STAGE == 1)
  {
    // Wait for response
    if (get_status() == VECTORDB_API_RESPONSE_DONE)
    {
      GATHERED_DOCUMENTS = PYTHON_QUESTION_RESPONSE_MUTEX.get_complete_response();
      set_status(VECTORDB_API_RESPONSE_READY_TO_GATHER);
    }
  }
}

void VECTORDB_PYTHON_API::submit_file_to_embed(string Collection_Name, string File)
{
  THINKING = true;
  ABOUT = "embed documents";
  THINKING_STAGE = 0;
  SUBJECT = File;
  COLLECTION_NAME = Collection_Name;
}

void VECTORDB_PYTHON_API::submit_clear_database()
{
  THINKING = true;
  ABOUT = "clear database";
  THINKING_STAGE = 0;
  SUBJECT = "";
  COLLECTION_NAME = "";
}

void VECTORDB_PYTHON_API::submit_list_database()
{
  THINKING = true;
  ABOUT = "list database";
  THINKING_STAGE = 0;
  SUBJECT = "";
  COLLECTION_NAME = "";
}

void VECTORDB_PYTHON_API::search_db_for_relevant_docs(string Search_Criteria, string Collection_Name)
{
  THINKING = true;
  ABOUT = "gathering information";
  THINKING_STAGE = 0;
  SUBJECT = Search_Criteria;
  COLLECTION_NAME = Collection_Name;
}

bool VECTORDB_PYTHON_API::get_gathered_documents(string &Documents_Gathered)
{
  if (GATHERED_DOCUMENTS_RELEVANT)
  {
    Documents_Gathered = GATHERED_DOCUMENTS;
    clear_thoughts();
    return true;
  }
  else
  {
    clear_thoughts();
    return false;
  }
}

string VECTORDB_PYTHON_API::get_full_response()
{
  string full_response = GATHERED_DOCUMENTS;
  clear_thoughts();
  return full_response;
}

void VECTORDB_PYTHON_API::process()
{
  if (THINKING)
  {
    if (ABOUT == "gathering information")
    {
      process_gathering_information_stages();
    }
    else if (ABOUT == "list database")
    {
      process_list_database_stages();
    }
    else if (ABOUT == "embed documents")
    {
      process_embed_documents_stages();
    }
    else if (ABOUT == "clear database")
    {
      process_clear_database_stages();
    }
  }
}

// ------------------------------------------------------------------------- //

#endif // VECTORDB_PYTHON_API_CPP
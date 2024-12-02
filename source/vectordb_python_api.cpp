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
  APP_TYPE = "e";

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

void VECTORDB_PYTHON_API::submit_question_to_ollama(string Question, string App_Type) 
{
  APP_TYPE = App_Type;
  STAGE = 0;
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

void VECTORDB_PYTHON_API::submit_question_to_ollama_par(string Question, string App_Type, OLLAMA_API &Ollama_System) 
{
  STAGE = 0;

  {
    Ollama_System.submit_question(Question);
  }

  {
    APP_TYPE = App_Type;
    
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
}

void VECTORDB_PYTHON_API::submit_file_to_embed(string File)
{
  DOCS_ONLY = true;

  APP_TYPE = "m";
  
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
  DOCS_ONLY = true;

  APP_TYPE = "b";
  
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
  DOCS_ONLY = true;

  APP_TYPE = "d";
  
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
  // For Responses that pull information from vector database, close
  if (DOCS_ONLY)
  {
    if (PYTHON_QUESTION_RESPONSE_MUTEX.done() == VECTORDB_API_RESPONS_GENERATING)
    {
      //Output.add_to("generating docs", Focus);
    }
    else if (PYTHON_QUESTION_RESPONSE_MUTEX.done() == VECTORDB_API_RESPONSE_DONE)
    {
      if (APP_TYPE == "d" || APP_TYPE == "m" || APP_TYPE == "b")
      {
        string ret_response = "";

        if (PYTHON_QUESTION_RESPONSE_MUTEX.response_size() > 0)
        {
          vector<string> result;
          PYTHON_QUESTION_RESPONSE_MUTEX.get_response_to_vector(result);
          ret_response = return_vector_as_string(result);
        }

        Output.add_to(ret_response, Focus);

        // Post Process Here
        if (Ollama_System.get_status() == OLLAMA_API_READY_FOR_REQUEST)
        {
          string request_to_summerize = "Summerize the following console output: " + PYTHON_QUESTION_RESPONSE_MUTEX.get_complete_response();
          
          Ollama_System.submit_question(request_to_summerize);
          
          QUESTION = "";
          APP_TYPE = "";
          PYTHON_QUESTION_RESPONSE_MUTEX.set_done(VECTORDB_API_READY_FOR_REQUEST);
        }
      }

      else if (APP_TYPE == "i")
      {
        // Post Process Here
        string question_with_docs = QUESTION + " - Answer that question using the following text as a resource: " + PYTHON_QUESTION_RESPONSE_MUTEX.get_complete_response();

        Ollama_System.submit_question(question_with_docs);

        QUESTION = "";
        APP_TYPE = "";
        PYTHON_QUESTION_RESPONSE_MUTEX.set_done(VECTORDB_API_READY_FOR_REQUEST);
      }

      else if (APP_TYPE == "n")
      {
        if (STAGE == 0)
        {
          if (Ollama_System.get_status() == OLLAMA_API_READY_FOR_REQUEST)
          {
            //string question_with_docs = "Answer with only \"YES\" or \"NO\". Is any of the information in the following text relevant to the topic? The following text is: " + 
            //                            PYTHON_QUESTION_RESPONSE_MUTEX.get_complete_response();
            //string question_with_docs = "Answer with only \"YES\" or \"NO\". Considering \"" + QUESTION + "\"  Is anything in the following text relevant: " + 
            //                            PYTHON_QUESTION_RESPONSE_MUTEX.get_complete_response();
            string question_with_docs = "Answer with only \"YES\" or \"NO\". Considering \"" + QUESTION + "\"  Is anything in the following text included: " + 
                                        PYTHON_QUESTION_RESPONSE_MUTEX.get_complete_response();

            Ollama_System.submit_question_internally(question_with_docs);

            STAGE = 1;
          }
        }
        else if (STAGE == 1)
        {
          if (Ollama_System.get_status() == OLLAMA_API_READY_FOR_REQUEST)
          {
            string answer = Ollama_System.get_complete_text_response();
            
            answer = trim(answer);
            answer = string_to_lower_case(answer);

            if (string_contains_word(answer, "yes"))
            {
              STAGE = 2;
            }
            else
            {
              APP_TYPE = "";
              QUESTION = "";
              PYTHON_QUESTION_RESPONSE_MUTEX.set_done(VECTORDB_API_READY_FOR_REQUEST);
              STAGE = 0;
            }
          }
        }
        else if (STAGE == 2)
        {
          // Post Process Here
          if (Ollama_System.get_status() == OLLAMA_API_READY_FOR_REQUEST)
          {
            //string question_with_docs = "Continue only relvevant information from the following text as a resource: " + PYTHON_QUESTION_RESPONSE_MUTEX.get_complete_response();
            //string question_with_docs = "Continue with '" + QUESTION + "' from the following text as a resource: " + PYTHON_QUESTION_RESPONSE_MUTEX.get_complete_response();
            //string question_with_docs = "Stay relevat when and continue with following text as a resource: " + PYTHON_QUESTION_RESPONSE_MUTEX.get_complete_response();
            //string question_with_docs = "Also include the following text as a resource: " + PYTHON_QUESTION_RESPONSE_MUTEX.get_complete_response();
            //string question_with_docs = "While responding to \"" + QUESTION + "\" with the following text. Don't repeat anything already said and " + 
            //                              "stay relevant to the topic. The following text is: " + PYTHON_QUESTION_RESPONSE_MUTEX.get_complete_response();

            string question_with_docs = "While responding to \"" + QUESTION + "\" with the following text. Don't repeat anything already said and " + 
                                          "filter out anything not relevant to the topic. The following text is: " + PYTHON_QUESTION_RESPONSE_MUTEX.get_complete_response();

            //string question_with_docs = "While responding to \"" + QUESTION + "\" with the following text. Don't repeat anything already said and " + 
            //                              "filter out anything not relevant to the topic. The following text is: " + PYTHON_QUESTION_RESPONSE_MUTEX.get_complete_response();

            Ollama_System.submit_question(question_with_docs);

            APP_TYPE = "";
            QUESTION = "";
            PYTHON_QUESTION_RESPONSE_MUTEX.set_done(VECTORDB_API_READY_FOR_REQUEST);
            STAGE = 3;
          }
        }

      }
    }
  }
  
  // These are inline resonses not associated to docs only, closes regardless of type.
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
      Output.seperater(Focus);
      
      // Post Process Here
      APP_TYPE = "";
      QUESTION = "";
      PYTHON_QUESTION_RESPONSE_MUTEX.set_done(VECTORDB_API_READY_FOR_REQUEST);
    }
  }
}



// ------------------------------------------------------------------------- //

#endif // VECTORDB_PYTHON_API_CPP
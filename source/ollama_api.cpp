#ifndef OLLAMA_API_CPP
#define OLLAMA_API_CPP

#include "ollama_api.h"

using namespace std;

// ------------------------------------------------------------------------- //

void OLLAMA_API::on_receive_response(const ollama::response& response) 
{
  if (response.as_json()["done"] == true) 
  {
    OLLAMA_MUTEX.add_to_response(response);
    OLLAMA_MUTEX.set_done(4);
  }
  else
  {
    OLLAMA_MUTEX.add_to_response(response);
  }
}

void OLLAMA_API::proc_render_thread() 
{
  OLLAMA_MUTEX.set_done(OLLAMA_API_RESPONS_GENERATING);

  // Set the callback to the member function
  std::function<void(const ollama::response&)> response_callback = [this](const ollama::response& res) 
  {
    this->on_receive_response(res);
  };

  // Generate the response
  ollama::generate("llama3:8b", OLLAMA_MUTEX.request(), response_callback);
}

// ------------------------------------------------------------------------- //

bool OLLAMA_API::create()
{
  bool ret_running = false;

  // Optional. By default, the server URL is set to http://localhost:11434. 
  // Use this function if your server resides at a different URL.
  OLLAMA.setServerURL("http://localhost:11434");
  //OLLAMA.setServerURL("http://127.0.0.1:11434");
  
  // Optional. Set the read and write timeouts in seconds for server interactions.
  // If you have a large model with a long response time you may need to increase these.
  OLLAMA.setReadTimeout(120);
  OLLAMA.setWriteTimeout(120);

  if (OLLAMA.is_running())
  {
    cout << "Version: " << OLLAMA.get_version() << endl;

    // This can optionally be used to deliberately load a model into memory prior to use. 
    // This occurs automatically when a request is made for an unloaded model, but can be 
    // useful for loading a model in advance.
    // bool model_loaded = OLLAMA.load_model("llama3:8b");

    // Pull a model by specifying a model name.
    // bool model_pulled = OLLAMA.pull_model("llama3:8b");

    // Copy a model by specifying a source model and destination model name.
    // bool model_copied = OLLAMA.copy_model("llama3:8b", "llama3_copy");

    // Delete a model by specifying a model name.
    // bool model_deleted = OLLAMA.delete_model("llama3_copy");

    // Request model info from the OLLAMA server.
    nlohmann::json model_info = OLLAMA.show_model_info("llama3:8b");
    cout << "Model family is " << model_info["details"]["family"] << endl;

    // List the models available locally in the OLLAMA server
    OLLAMA_MODELS_LIST = OLLAMA.list_models();
    print_vector_with_title("Available models", OLLAMA_MODELS_LIST);

    // List the models available locally in the OLLAMA server
    OLLAMA_MODELS_RUNNING = OLLAMA.list_running_models();
    print_vector_with_title("Running models", OLLAMA_MODELS_RUNNING);

    // Most calls will throw ollama::exception in the event of an error, with details on the exception that has occurred. Exceptions are enabled by default.    
    //try { 
    //    OLLAMA.generate("Non-existent-model", "Requesting this model will throw an error"); 
    //} 
    //catch(OLLAMA.exception e) { cout << e.what() << endl; }

    // You can also dynamically enable and disable exceptions. If exceptions are disabled, 
    // functions will return an empty OLLAMA.response or false where appropriate instead 
    // of throwing OLLAMA.exception.
    //OLLAMA.allow_exceptions(false);

    cout  << endl;

    ret_running = true;
  }

  return ret_running;
}

int OLLAMA_API::get_status()
{
  return OLLAMA_MUTEX.done();
}

void OLLAMA_API::set_ready_for_request()
{
  OLLAMA_MUTEX.set_done(OLLAMA_API_READY_FOR_REQUEST);
}

void OLLAMA_API::set_questioning()
{
  OLLAMA_MUTEX.set_done(OLLAMA_API_WRITING_REQUEST);
}

void OLLAMA_API::set_request(const string& Request)
{
  OLLAMA_MUTEX.set_request(Request);
  OLLAMA_MUTEX.set_done(OLLAMA_API_REQUEST_SUBMITTED);
}

int OLLAMA_API::check_response()
{
  OLLAMA_MUTEX.get_response_to_vector(RESPONSE);
  return RESPONSE.size();
}

// ------------------------------------------------------------------------- //

#endif  // OLLAMA_API_CPP
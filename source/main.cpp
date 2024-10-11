#ifndef MAIN_CPP
#define MAIN_CPP

#include "main.h"

using namespace std;

int main()
{
  SYSTEM system_d;

  bool exit = false;
  string input = "";

  // Optional. By default, the server URL is set to http://localhost:11434. 
  // Use this function if your server resides at a different URL.
  system_d.OLLAMA.setServerURL("http://localhost:11434");
  
  // Optional. Set the read and write timeouts in seconds for server interactions.
  // If you have a large model with a long response time you may need to increase these.
  system_d.OLLAMA.setReadTimeout(120);
  system_d.OLLAMA.setWriteTimeout(120);

  if (system_d.OLLAMA.is_running())
  {
    cout << "Version: " << system_d.OLLAMA.get_version() << endl;

    // This can optionally be used to deliberately load a model into memory prior to use. 
    // This occurs automatically when a request is made for an unloaded model, but can be 
    // useful for loading a model in advance.
    // bool model_loaded = system_d.OLLAMA.load_model("llama3:8b");

    // Pull a model by specifying a model name.
    // bool model_pulled = system_d.OLLAMA.pull_model("llama3:8b");

    // Copy a model by specifying a source model and destination model name.
    // bool model_copied = system_d.OLLAMA.copy_model("llama3:8b", "llama3_copy");

    // Delete a model by specifying a model name.
    // bool model_deleted = system_d.OLLAMA.delete_model("llama3_copy");

    // Request model info from the system_d.OLLAMA server.
    nlohmann::json model_info = system_d.OLLAMA.show_model_info("llama3:8b");
    cout << "Model family is " << model_info["details"]["family"] << endl;

    // List the models available locally in the system_d.OLLAMA server
    system_d.OLLAMA_MODELS_LIST = system_d.OLLAMA.list_models();
    print_vector_with_title("Available models", system_d.OLLAMA_MODELS_LIST);

    // List the models available locally in the system_d.OLLAMA server
    system_d.OLLAMA_MODELS_RUNNING = system_d.OLLAMA.list_running_models();
    print_vector_with_title("Running models", system_d.OLLAMA_MODELS_RUNNING);

    // Most calls will throw ollama::exception in the event of an error, with details on the exception that has occurred. Exceptions are enabled by default.    
    //try { 
    //    system_d.OLLAMA.generate("Non-existent-model", "Requesting this model will throw an error"); 
    //} 
    //catch(system_d.OLLAMA.exception e) { cout << e.what() << endl; }

    // You can also dynamically enable and disable exceptions. If exceptions are disabled, 
    // functions will return an empty system_d.OLLAMA.response or false where appropriate instead 
    // of throwing system_d.OLLAMA.exception.
    //system_d.OLLAMA.allow_exceptions(false);

    cout  << endl;

    while (exit == false)
    {
      
      cout << "INPUT > " << endl;
      cin >> input;

      if (input != "exit")
      {
        cout << system_d.OLLAMA.generate("llama3:8b", input) << endl;
      }
      else
      {
        exit = true;
      }

    }
  }



  return 0;
}

#endif // MAIN_CPP
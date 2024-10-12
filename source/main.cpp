#ifndef MAIN_CPP
#define MAIN_CPP

#include "main.h"

using namespace std;

/*
std::atomic<bool> done{false};

void on_receive_response(const ollama::response& response)
{   
  std::cout << response << std::flush;

  if (response.as_json()["done"]==true)
  {
    done=true;  
    std::cout << std::endl;
  }
}

// Use std::function to define a callback from an existing function
// You can also use a lambda with an equivalent signature
std::function<void(const ollama::response&)> response_callback = on_receive_response;  

// You can launch the generation in a thread with a callback to use it asynchronously.
std::thread new_thread( [response_callback]{ 
  ollama::generate("llama3:8b", "Why is the sky blue?", response_callback); } );
*/



std::atomic<bool> done{false};
std::function<void(const ollama::response&)> response_callback;

void on_receive_response(const ollama::response& response) {
    // Print the token received
    std::cout << response << std::flush; 

    // Check if done and update atomically
    if (response.as_json()["done"] == true) {
        done = true;
        std::cout << std::endl;
    }
}

// Initialization function to set up the callback
void initialize_response_callback() {
    response_callback = on_receive_response;
}

void generate_request() {
    ollama::generate("llama3:8b", "Why is the sky blue?", response_callback);
}

/*
int main() {
    initialize_response_callback();

    // Thread to launch the generation asynchronously
    std::thread new_thread(generate_request);

    // Join the thread to ensure it completes
    new_thread.join();

    return 0;
}
*/






void send_to_ollama(SYSTEM &sdSystem, string Text)
{
  int test = 1;
  cout << "Sending: " << Text << endl;

  if (test == 1)
  {
    cout  << sdSystem.OLLAMA.generate("llama3:8b", Text) << endl;
  }

  if (test == 2)
  {
    //  All generative calls can include options specified through an ollama::options 
    //  object. This class extends nlohmann::json and can support the options 
    //  specified at 
    //  https://github.com/ollama/ollama/blob/main/docs/api.md#generate-request-with-options.
    
    // Access and set these options like any other json type.
    sdSystem.options["seed"] = 1;
    sdSystem.options["temperature"] = 0;
    sdSystem.options["num_predict"] = 18; 

    // Options can be included with any generative function
    ollama::response response = sdSystem.OLLAMA.generate("llama3:8b", Text, sdSystem.options);   
  }

  if (test == 3)
  {
    // Bind the callback to the generation
    sdSystem.OLLAMA.generate("llama3:8b", Text, response_callback);
  }

  if (test == 4)
  {
    sdSystem.responce = sdSystem.OLLAMA.generate("llama3:8b", Text, sdSystem.options);
  }

  if (test == 5)
  {
    // You can launch the generation in a thread with a callback to use it asynchronously.
    //std::thread new_thread( [response_callback]{ 
    //ollama::generate("llama3:8b", "Why is the sky blue?", response_callback); } );
    //ollama::generate("llama3:8b", "Why is the sky blue?", response_callback);

    
    // Thread to launch the generation asynchronously
    std::thread new_thread(generate_request);

    // Join the thread to ensure it completes
    new_thread.join();
  }

  if (test == 6)
  {

  }

}

int main()
{
  /*/
  future<void> thread_render; // The thread containing function to send the led color array 
                              //  to the leds.
  bool thread_render_running = false; // Set to true when thread is active.

  // Control for the output thread. 
  //  The only part of output asynced is the printf function of the player 1001 type.
  //  Everything else in output will be paused if async is actively running and does 
  //  not need to be asynced because the interface is fast enough to run within one 
  //  cycle.
  future<void> thread_output;         // The thread containing function to printf a large 
                                      //  string.
  //bool thread_output_running = false; // Set to true when thread is active.
  string raw_string_buffer = "";      // A string buffer to contain the Screen buffer. 
                                      //  Redundant. Possibly consolidate the line. 
  */

  initialize_response_callback();

  // Main System Varibles
  SYSTEM sdSystem;

    // Sleeping Loop Variables
  TIMED_IS_READY  ollama_sleep_timer;          // Delay for communicating with compass 
  ollama_sleep_timer.set( 1000 / OLLAMA_SLEEP_TIMER_DELAY );

  sdSystem.PROGRAM_TIME.create();    //  Get current time.  This will be our timeframe to work in.
  if (sdSystem.PROGRAM_TIME.setframetime() == true)
  {
    sdSystem.PROGRAM_TIME.clear_error();
  }

  bool exit = false;
  string input = "";

  // Optional. By default, the server URL is set to http://localhost:11434. 
  // Use this function if your server resides at a different URL.
  sdSystem.OLLAMA.setServerURL("http://localhost:11434");
  //sdSystem.OLLAMA.setServerURL("http://127.0.0.1:11434");
  
  // Optional. Set the read and write timeouts in seconds for server interactions.
  // If you have a large model with a long response time you may need to increase these.
  sdSystem.OLLAMA.setReadTimeout(120);
  sdSystem.OLLAMA.setWriteTimeout(120);

  if (sdSystem.OLLAMA.is_running())
  {
    cout << "Version: " << sdSystem.OLLAMA.get_version() << endl;

    // This can optionally be used to deliberately load a model into memory prior to use. 
    // This occurs automatically when a request is made for an unloaded model, but can be 
    // useful for loading a model in advance.
    // bool model_loaded = sdSystem.OLLAMA.load_model("llama3:8b");

    // Pull a model by specifying a model name.
    // bool model_pulled = sdSystem.OLLAMA.pull_model("llama3:8b");

    // Copy a model by specifying a source model and destination model name.
    // bool model_copied = sdSystem.OLLAMA.copy_model("llama3:8b", "llama3_copy");

    // Delete a model by specifying a model name.
    // bool model_deleted = sdSystem.OLLAMA.delete_model("llama3_copy");

    // Request model info from the sdSystem.OLLAMA server.
    nlohmann::json model_info = sdSystem.OLLAMA.show_model_info("llama3:8b");
    cout << "Model family is " << model_info["details"]["family"] << endl;

    // List the models available locally in the sdSystem.OLLAMA server
    sdSystem.OLLAMA_MODELS_LIST = sdSystem.OLLAMA.list_models();
    print_vector_with_title("Available models", sdSystem.OLLAMA_MODELS_LIST);

    // List the models available locally in the sdSystem.OLLAMA server
    sdSystem.OLLAMA_MODELS_RUNNING = sdSystem.OLLAMA.list_running_models();
    print_vector_with_title("Running models", sdSystem.OLLAMA_MODELS_RUNNING);

    // Most calls will throw ollama::exception in the event of an error, with details on the exception that has occurred. Exceptions are enabled by default.    
    //try { 
    //    sdSystem.OLLAMA.generate("Non-existent-model", "Requesting this model will throw an error"); 
    //} 
    //catch(sdSystem.OLLAMA.exception e) { cout << e.what() << endl; }

    // You can also dynamically enable and disable exceptions. If exceptions are disabled, 
    // functions will return an empty sdSystem.OLLAMA.response or false where appropriate instead 
    // of throwing sdSystem.OLLAMA.exception.
    //sdSystem.OLLAMA.allow_exceptions(false);

    cout  << endl;

    ollama::options options;

    // Access and set these options like any other json type.
    options["seed"] = 1;
    options["temperature"] = 0;
    options["num_predict"] = 18;

    // Options can be included with any generative function
    ollama::response response = ollama::generate("llama3:8b", "Why is the sky blue?", options);

    // Main Thread Loop
    while (exit == false)
    //while (exit == false && sdSystem.PROGRAM_TIME.current_frame_time() < 10000)
    {

      if (sdSystem.PROGRAM_TIME.setframetime() == true)
      {
        //cout << "Frame Time: Inteupted " << endl;
        //cout << "  Current Time Frame: " << to_string(sdSystem.PROGRAM_TIME.current_frame_time()) << endl;
        //cout << "          Error (ms): " << to_string(sdSystem.PROGRAM_TIME.error()) << endl;

        sdSystem.PROGRAM_TIME.clear_error();
      }

      if (ollama_sleep_timer.is_ready(sdSystem.PROGRAM_TIME.current_frame_time()) == true)
      {
        // ---

        cout << to_string(sdSystem.PROGRAM_TIME.current_frame_time()) << " Loop Start" << endl << endl;

        /*
        // ---

        // Close all completed and active threads after sleep cycle is complete.

        // Before starting a new loop, close the render thread from the previous loop, if 
        //  render is complete
        if(thread_render_running == true)
        // Check to see if render thread was started before checking the completion status.
        {
          if(thread_render.wait_for(0ms) == future_status::ready)
          // Check to verify the thread is complete before allowing the render to start again. 
          {
            thread_render_running = false;
          }
        }
        */

        // ---

        if (input == "exit" || input == "bye")
        {
          exit = true;
        }
        else
        {
          if (sdSystem.FULL_RESPONSE_RECEIVED == 0)
          {
            
            cout << to_string(sdSystem.PROGRAM_TIME.current_frame_time()) << " Enter Input" << endl;

            cout << "INPUT > " << endl;
            getline(cin, input);

            send_to_ollama(sdSystem, input);
            
            sdSystem.FULL_RESPONSE_RECEIVED = 1;
            
            cout << to_string(sdSystem.PROGRAM_TIME.current_frame_time()) << " Exit Input"  << endl;
          }

          if (sdSystem.FULL_RESPONSE_RECEIVED == 1)
          {
            
            cout << to_string(sdSystem.PROGRAM_TIME.current_frame_time()) << " Enter Response"  << endl;
            // Check on Responses.

            // Print the token received
            //std::cout << sdSystem.responce << flush;

            // The server will set "done" to true for the last response
            if (response.as_json()["done"]==true)
            {
              cout << "---DONE---" << endl;

              cout << to_string(sdSystem.PROGRAM_TIME.current_frame_time()) << " Response Done" << endl << endl;
              sdSystem.FULL_RESPONSE_RECEIVED = 0;
            }
          }
        }
      }

      sdSystem.PROGRAM_TIME.request_ready_time(ollama_sleep_timer.get_ready_time());
      sdSystem.PROGRAM_TIME.sleep_till_next_frame();

    }
  }
  else
  {
    cout << "No server available." << endl;
  }


  return 0;
}

#endif // MAIN_CPP
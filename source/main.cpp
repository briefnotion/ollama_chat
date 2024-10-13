#ifndef MAIN_CPP
#define MAIN_CPP

#include "main.h"

using namespace std;

// ------------------------------------------------------------------------- //

OLLAMA_API_MUTEX OLLAMA_API;

// ------------------------------------------------------------------------- //

void on_receive_response(const ollama::response& response)
{   
  OLLAMA_API.add_to_response(response);

  if (response.as_json()["done"]==true) 
  { 
    OLLAMA_API.set_done(4);
  }
}


// Use std::function to define a callback from an existing function
// You can also use a lambda with an equivalent signature
std::function<void(const ollama::response&)> response_callback = on_receive_response; 


void proc_render_thread()
{
  cout << "Start Response: " << endl;

  ollama::generate("llama3:8b", OLLAMA_API.request(), response_callback);
}


// ------------------------------------------------------------------------- //

bool ollama_create(SYSTEM &sdSystem)
{
  bool ret_running = false;

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

    ret_running = true;
  }

  return ret_running;
}

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //

int main()
{
  // Print Revision
  cout << "Ollama Chat Revision: " << Revision << endl << endl;

  // ------------------------------------------------------------------------- //

  // Main System Varibles
  SYSTEM sdSystem;

  // ------------------------------------------------------------------------- //

  // Prepair Render Threads
  future<void> thread_render; // The thread containing function to send.

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

  // Generate info for threads to be active.
  sdSystem.TEST_THREAD_INFO.create(TEST_THREAD_TIMER_DELAY);

  // ------------------------------------------------------------------------- //

  //initialize_response_callback();

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

  // Only run main look if connection to server is sucessful.
  if (ollama_create(sdSystem))
  {
    // Main Thread Loop
    while (exit == false)
    //while (exit == false && sdSystem.PROGRAM_TIME.current_frame_time() < 15000)
    {
      // Main loop set up and start
      if (sdSystem.PROGRAM_TIME.setframetime() == true)
      {
        //cout << "Frame Time: Inteupted " << endl;
        //cout << "  Current Time Frame: " << to_string(sdSystem.PROGRAM_TIME.current_frame_time()) << endl;
        //cout << "          Error (ms): " << to_string(sdSystem.PROGRAM_TIME.error()) << endl;

        sdSystem.PROGRAM_TIME.clear_error();
      }

      // ------------------------------------------------------------------------- //

      //cout << "Main Loop Start: " << to_string(sdSystem.PROGRAM_TIME.current_frame_time()) << endl;

      // Threading
      // ------------------------------------------------------------------------- //

      // Close all completed and active threads after sleep cycle is complete.

      // Before starting a new loop, close the render thread from the previous loop, if 
      //  render is complete
      if(sdSystem.TEST_THREAD_INFO.IS_RUNNING == true)
      // Check to see if render thread was started before checking the completion status.
      {
        if(thread_render.wait_for(0ms) == future_status::ready)
        // Check to verify the thread is complete before allowing the render to start again. 
        {
          //cout << "Thread closed" << endl;
          sdSystem.TEST_THREAD_INFO.IS_RUNNING = false;
        }
        else
        {
          //cout << "Thread still open" << endl;
        }
      }

      // ------------------------------------------------------------------------- //

      // Ollama check routine
      //  Never comment this out or the system will never sleep
      if (ollama_sleep_timer.is_ready(sdSystem.PROGRAM_TIME.current_frame_time()) == true)
      {
        //cout << to_string(sdSystem.PROGRAM_TIME.current_frame_time()) << " Loop Start" << endl << endl;

        // Get to next line if response found and ask another question
        if (OLLAMA_API.done() == 4)
        {
          cout << endl << endl;

          // Response Done
          OLLAMA_API.set_done(0);
        }

        // ask a question, exit if needed.
        if (OLLAMA_API.done() == 0)
        {
          // writing question
          OLLAMA_API.set_done(1);

          cout << "INPUT > " << endl;
          getline(cin, input);
          
          if (input == "exit" || input == "bye")
          {
            exit = true;
          }
          else
          {
            // submit request
            OLLAMA_API.set_request(input);

            // question waiting
            OLLAMA_API.set_done(2);
          }
        }

        // Print Responses that may arrive.
        if (OLLAMA_API.response_size() > 0)
        {
          cout << OLLAMA_API.return_vector_as_string() << flush;
        }
      }

      // ------------------------------------------------------------------------- //

      // Run async Ollama if question waiting and async not running.
      //  Never comment this out or the system will never sleep
      if (sdSystem.TEST_THREAD_INFO.is_ready(sdSystem.PROGRAM_TIME.current_frame_time()) == true)
      {
        if (OLLAMA_API.done() == 2)
        {
          if (sdSystem.TEST_THREAD_INFO.IS_RUNNING == false)
          {
            // start generating response
            OLLAMA_API.set_done(3);

            thread_render = async(proc_render_thread);
            sdSystem.TEST_THREAD_INFO.IS_RUNNING = true;
          }
        }
      }

      // ------------------------------------------------------------------------- //
      // Make sure non of these are commented out, or the system will never sleep.
      sdSystem.PROGRAM_TIME.request_ready_time(ollama_sleep_timer.get_ready_time());
      sdSystem.PROGRAM_TIME.request_ready_time(sdSystem.TEST_THREAD_INFO.get_ready_time());
      sdSystem.PROGRAM_TIME.sleep_till_next_frame();

    } // end of main loop

    // ------------------------------------------------------------------------- //
    // Shutdown any open threads process
    if(sdSystem.TEST_THREAD_INFO.IS_RUNNING == true)
    // Check to see if render thread was started before checking the completion status.
    {
      cout << endl << "Closing open threads" << endl << endl;

      while(thread_render.wait_for(10ms) != future_status::ready)
      {
        //cout << "Waiting for Render Thread to finish" << endl;
      }

      sdSystem.TEST_THREAD_INFO.IS_RUNNING = false;
    }
    else
    {
      cout << endl << "No open threads to close" << endl << endl;
    }
  }
  else
  {
    cout << endl << "No server available." << endl << endl;
  }

  return 0;
}

// ------------------------------------------------------------------------- //

#endif // MAIN_CPP
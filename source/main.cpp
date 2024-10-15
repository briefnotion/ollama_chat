#ifndef MAIN_CPP
#define MAIN_CPP

#include "main.h"

using namespace std;

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
  sdSystem.OLLAMA_RESPONSE_THREAD.create(TEST_THREAD_TIMER_DELAY);

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

  // Only run main look if connection to server is sucessful.
  if (sdSystem.OLLAMA_SYSTEM.create())
  {
    // Main Thread Loop
    while (exit == false)
    //while (exit == false && sdSystem.PROGRAM_TIME.current_frame_time() < 15000)
    {
      // Main loop set up and start
      if (sdSystem.PROGRAM_TIME.setframetime() == true)
      {
        sdSystem.PROGRAM_TIME.clear_error();
      }

      // ------------------------------------------------------------------------- //

      //cout << "Main Loop Start: " << to_string(sdSystem.PROGRAM_TIME.current_frame_time()) << endl;

      // Threading
      // ------------------------------------------------------------------------- //

      // Close all completed and active threads after sleep cycle is complete.
      sdSystem.OLLAMA_RESPONSE_THREAD.check_for_completition();

      // ------------------------------------------------------------------------- //

      // Ollama check routine
      //  Never comment this out or the system will never sleep
      if (ollama_sleep_timer.is_ready(sdSystem.PROGRAM_TIME.current_frame_time()) == true)
      {
        // Get to next line if response found and ask another question
        if (sdSystem.OLLAMA_SYSTEM.get_status() == OLLAMA_API_RESPONSE_DONE)
        {
          cout << endl << endl;
          sdSystem.OLLAMA_SYSTEM.set_ready_for_request();
        }

        // ask a question, exit if needed.
        if (sdSystem.OLLAMA_SYSTEM.get_status() == OLLAMA_API_READY_FOR_REQUEST)
        {
          sdSystem.OLLAMA_SYSTEM.set_questioning();

          string question;
          question = ask_a_question("INPUT > ");
          
          if (question == "exit" || question == "bye")
          {
            exit = true;
          }
          else
          {
            // submit request
            sdSystem.OLLAMA_SYSTEM.set_request(question);
          }
        }

        // Print Responses that may arrive.
        if (sdSystem.OLLAMA_SYSTEM.check_response() > 0)
        {
          cout << return_vector_as_string(sdSystem.OLLAMA_SYSTEM.RESPONSE_STRING_VECTOR) << flush;
        }
        
        sdSystem.OLLAMA_SYSTEM.check_response_done();
      }

      //cout << to_string(sdSystem.OLLAMA_SYSTEM.get_status()) << flush;

      // ------------------------------------------------------------------------- //

      // Run async Ollama if question waiting and async not running.
      //  Never comment this out or the system will never sleep
      if(sdSystem.OLLAMA_RESPONSE_THREAD.check_to_run_routine_on_thread(sdSystem.PROGRAM_TIME.current_frame_time()))
      {
        if (sdSystem.OLLAMA_SYSTEM.get_status() == OLLAMA_API_REQUEST_SUBMITTED)
        {
          // start generating response
          cout << endl << "OUTPUT > " << endl;
          
          // Be careful with this because it looks like black magic to me.
          sdSystem.OLLAMA_RESPONSE_THREAD.start_render_thread([&]() 
                        {  sdSystem.OLLAMA_SYSTEM.proc_render_thread();  });
        }
      }

      // ------------------------------------------------------------------------- //
      // Make sure non of these are commented out, or the system will never sleep.
      sdSystem.PROGRAM_TIME.request_ready_time(ollama_sleep_timer.get_ready_time());
      sdSystem.PROGRAM_TIME.request_ready_time(sdSystem.OLLAMA_RESPONSE_THREAD.get_ready_time());
      sdSystem.PROGRAM_TIME.sleep_till_next_frame();

    } // end of main loop

    // ------------------------------------------------------------------------- //
    // Shutdown any open threads process
    sdSystem.OLLAMA_RESPONSE_THREAD.wait_for_thread_to_finish();
  }
  else
  {
    cout << endl << "No server available." << endl << endl;
  }

  return 0;
}

// ------------------------------------------------------------------------- //

#endif // MAIN_CPP
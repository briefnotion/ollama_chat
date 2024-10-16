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

  // Prepair Variables:
  sdSystem.INPUT.create();

  sdSystem.OUTPUT_INPUT.create(0, 0, 0, 5);
  sdSystem.OUTPUT_OLLAMA_RESPONSE.create(1, 0, 5, 10);

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

  bool main_loop_exit = false;

  // Only run main look if connection to server is sucessful.
  if (sdSystem.OLLAMA_SYSTEM.create(sdSystem.OUTPUT_OLLAMA_RESPONSE, sdSystem.OUTPUT_FOCUS))
  {
    // Send the output of the create to the screen.
    sdSystem.OUTPUT_OLLAMA_RESPONSE.output(sdSystem.OUTPUT_FOCUS);

    // Main Thread Loop
    while (main_loop_exit == false)
    //while (exit == false && sdSystem.PROGRAM_TIME.current_frame_time() < 15000)
    {
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
      }

      // ------------------------------------------------------------------------- //

      // Ollama check routine
      //  Never comment this out or the system will never sleep
      if (ollama_sleep_timer.is_ready(sdSystem.PROGRAM_TIME.current_frame_time()) == true)
      {
        // Get to next line if response found and ask another question
        if (sdSystem.OLLAMA_SYSTEM.get_status() == OLLAMA_API_RESPONSE_DONE)
        {
          sdSystem.OUTPUT_OLLAMA_RESPONSE.add_to("\n\n-----\n", sdSystem.OUTPUT_FOCUS);
          sdSystem.OLLAMA_SYSTEM.set_status(OLLAMA_API_READY_FOR_REQUEST);
        }

        // preparations for next question
        if (sdSystem.OLLAMA_SYSTEM.get_status() == OLLAMA_API_READY_FOR_REQUEST)
        {
          sdSystem.OUTPUT_INPUT.clear();
          sdSystem.OLLAMA_SYSTEM.set_status(OLLAMA_API_WRITING_REQUEST);
        }

        // start getting input.
        //if (sdSystem.OLLAMA_SYSTEM.get_status() == OLLAMA_API_WRITING_REQUEST)
        if (true)
        {
          sdSystem.INPUT.read_input(sdSystem.OUTPUT_INPUT, sdSystem.OUTPUT_FOCUS);

          if (sdSystem.OUTPUT_INPUT.CHANGED)
          {
            //sdSystem.OUTPUT_INPUT.output(sdSystem.OUTPUT_FOCUS);
            
            if (sdSystem.OUTPUT_INPUT.pressed_enter())
            {
              sdSystem.OLLAMA_SYSTEM.set_status(OLLAMA_API_RESPONS_GENERATING);
              string input_entered = sdSystem.OUTPUT_INPUT.value();

              if (input_entered == "exit" || input_entered == "bye")
              {
                main_loop_exit = true;
              }
              else
              {
                // submit request
                sdSystem.OLLAMA_SYSTEM.set_request(input_entered);
              }
            }
          }
        }

        // Print Responses that may arrive.
        if (sdSystem.OLLAMA_SYSTEM.check_response() > 0)
        {
          sdSystem.OUTPUT_OLLAMA_RESPONSE.add_to(return_vector_as_string(sdSystem.OLLAMA_SYSTEM.RESPONSE_STRING_VECTOR), 
                                                                          sdSystem.OUTPUT_FOCUS);

        }

        sdSystem.OUTPUT_INPUT.output(sdSystem.OUTPUT_FOCUS);
        sdSystem.OUTPUT_OLLAMA_RESPONSE.output(sdSystem.OUTPUT_FOCUS);

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
    // Restore the terminal
    sdSystem.OLLAMA_RESPONSE_THREAD.wait_for_thread_to_finish();
    sdSystem.INPUT.restore_terminal_settings();

  }
  else
  {
    cout << endl << "No server available." << endl << endl;
  }

  return 0;
}

// ------------------------------------------------------------------------- //

#endif // MAIN_CPP
#ifndef MAIN_CPP
#define MAIN_CPP

#include "main.h"

using namespace std;

// ------------------------------------------------------------------------- //

int main()
{
  string version_info = Revision;
  version_info = "Ollama Chat Revision: " + version_info + "\n\n";
  // ------------------------------------------------------------------------- //

  // Main System Varibles
  SYSTEM sdSystem;

  // ------------------------------------------------------------------------- //
  // Testing

  /*
  int rows = 0;
  int cols = 0;
  get_console_size(rows, cols);

  cout << "Screen Size " << rows << ":" << cols << endl;
  */

  int ret_code = 0;
  
  //ret_code = sdSystem.VECTORDB_SYSTEM.embedding_test_python_1();
  //ret_code = sdSystem.VECTORDB_SYSTEM.embedding_test_python_2();
  //ret_code = sdSystem.VECTORDB_SYSTEM.embedding_test_python_3();

  //ret_code = sdSystem.VECTORDB_SYSTEM.embedding_test_1();
  //ret_code = sdSystem.VECTORDB_SYSTEM.embedding_test_2();
  //ret_code = sdSystem.VECTORDB_SYSTEM.embedding_test_3();
  
  //ret_code = sdSystem.CHROMADB_SYSTEM.embedding_test_1();

  //if (ret_code == 1 || ret_code == 0)

  if (ret_code == 1)
  {
    exit(ret_code);
  }

  // ------------------------------------------------------------------------- //
  // Initialize Timers 
  sdSystem.SCREENIO_SLEEP_TIMER.set( 1000 / SCREENIO_SLEEP_TIMER_DELAY );
  sdSystem.OLLAMA_SLEEP_TIMER.set( 1000 / OLLAMA_SLEEP_TIMER_DELAY );
  sdSystem.EMBEDDING_SLEEP_TIMER.set( 1000 / EMBEDDING_SLEEP_TIMER_DELAY );

  sdSystem.PROGRAM_TIME.create();    //  Get current time.  This will be our timeframe to work in.
  if (sdSystem.PROGRAM_TIME.setframetime() == true)
  {
    sdSystem.PROGRAM_TIME.clear_error();
  }

  bool main_loop_exit = false;

  // Only run main look if connection to server is sucessful.
  sdSystem.OLLAMA_SYSTEM.PROPS.MODEL = "llama3.1:8b";
  sdSystem.OLLAMA_SYSTEM.PROPS.URL = "http://localhost:11434";
    
  // Print Revision
  sdSystem.OUTPUT_OLLAMA_RESPONSE.add_to(version_info, sdSystem.OUTPUT_FOCUS);

  if (sdSystem.OLLAMA_SYSTEM.create(sdSystem.OUTPUT_OLLAMA_RESPONSE, sdSystem.OUTPUT_FOCUS))
  {
    // ------------------------------------------------------------------------- //

    // Prepair Variables:
    sdSystem.INPUT.create();

    sdSystem.OUTPUT_CLOCK.PROPS.POSITION_X = 0;
    sdSystem.OUTPUT_CLOCK.PROPS.POSITION_Y = 0;
    sdSystem.OUTPUT_CLOCK.PROPS.LINES = 1;
    sdSystem.OUTPUT_CLOCK.create(0);
    string clock = "-";
    int clock_clounter = 0;

    //sdSystem.OUTPUT_INPUT.PROPS.TITLE = "  INPUT";
    sdSystem.OUTPUT_INPUT.PROPS.POSITION_X = 0;
    sdSystem.OUTPUT_INPUT.PROPS.POSITION_Y = 1;
    sdSystem.OUTPUT_INPUT.PROPS.LINES = 2;
    sdSystem.OUTPUT_INPUT.create(1);

    sdSystem.OUTPUT_OLLAMA_RESPONSE.PROPS.TITLE = "  OUTPUT";
    sdSystem.OUTPUT_OLLAMA_RESPONSE.PROPS.POSITION_X = 0;
    sdSystem.OUTPUT_OLLAMA_RESPONSE.PROPS.POSITION_Y = 3;
    sdSystem.OUTPUT_OLLAMA_RESPONSE.PROPS.LINES = 100;
    sdSystem.OUTPUT_OLLAMA_RESPONSE.create(2);
    
    sdSystem.INPUT.clear_screeen();

    // ------------------------------------------------------------------------- //

    sdSystem.OLLAMA_RESPONSE_THREAD.create(OLLAMA_RESPONSE_THREAD_TIMER_DELAY);

    // Send the output of the create to the screen.
    sdSystem.OUTPUT_OLLAMA_RESPONSE.output(sdSystem.OUTPUT_FOCUS);

    // ------------------------------------------------------------------------- //
    // ------------------------------------------------------------------------- //

    // Main Thread Loop
    while (main_loop_exit == false)
    //while (exit == false && sdSystem.PROGRAM_TIME.current_frame_time() < 15000)
    {
      {
        // Main loop set up and start
        if (sdSystem.PROGRAM_TIME.setframetime() == true)
        {
          sdSystem.OUTPUT_OLLAMA_RESPONSE.add_to("CLOCK SKEW DETECTED\n", sdSystem.OUTPUT_FOCUS);
          sdSystem.PROGRAM_TIME.clear_error();
        }

        // ------------------------------------------------------------------------- //

        //cout << "Main Loop Start: " << to_string(sdSystem.PROGRAM_TIME.current_frame_time()) << endl;
        //cout << "." << flush;
        
        // Threading
        // ------------------------------------------------------------------------- //

        // Close all completed and active threads after sleep cycle is complete.
        sdSystem.OLLAMA_RESPONSE_THREAD.check_for_completition();
      }

      {
        clock_clounter++;

        if (clock_clounter >= 100)
        {
          // tick - tock
          clock_clounter = 0;

          if (clock == "|")
          {
            clock = "/";
          }
          else if (clock == "/")
          {
            clock = "-";
          }
          else if (clock == "-")
          {
            clock = "\\";
          }
          else 
          {
            clock = "|";
          }

          sdSystem.OUTPUT_CLOCK.clear();
          sdSystem.OUTPUT_CLOCK.redraw();
          sdSystem.OUTPUT_CLOCK.add_to(linemerge_left_justify("---------------------------------", clock + " INPUT:"), sdSystem.OUTPUT_FOCUS);
        }
      }

      // ------------------------------------------------------------------------- //
      // Ollama check routine

      //  Never comment this out or the system will never sleep
      if (sdSystem.OLLAMA_SLEEP_TIMER.is_ready(sdSystem.PROGRAM_TIME.current_frame_time()) == true)
      {
        // Print Responses that may arrive.
        if (sdSystem.OLLAMA_SYSTEM.check_response() > 0)
        {
          sdSystem.OUTPUT_OLLAMA_RESPONSE.add_to(return_vector_as_string(sdSystem.OLLAMA_SYSTEM.RESPONSE_STRING_VECTOR), 
                                                                          sdSystem.OUTPUT_FOCUS);
        }

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

        sdSystem.OLLAMA_SYSTEM.check_response_done();
      }

      //cout << to_string(sdSystem.OLLAMA_SYSTEM.get_status()) << flush;

      // ------------------------------------------------------------------------- //
      // Ollama thread update check

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
      // Embedding check routine

      //  Never comment this out or the system will never sleep
      if (sdSystem.EMBEDDING_SLEEP_TIMER.is_ready(sdSystem.PROGRAM_TIME.current_frame_time()) == true)
      {
        sdSystem.OUTPUT_OLLAMA_RESPONSE.add_to(sdSystem.VECTORDB_SYSTEM.update(), sdSystem.OUTPUT_FOCUS);
      }

      // ------------------------------------------------------------------------- //
      // Screen IO Check.

      //  Never comment this out or the system will never sleep
      if (sdSystem.SCREENIO_SLEEP_TIMER.is_ready(sdSystem.PROGRAM_TIME.current_frame_time()) == true)
      {
        sdSystem.INPUT.read_input(sdSystem.OUTPUT_INPUT, sdSystem.OUTPUT_FOCUS);

        if (sdSystem.OUTPUT_INPUT.CHANGED)
        {
          if (sdSystem.OUTPUT_INPUT.pressed_enter())
          {
            string input_entered = sdSystem.OUTPUT_INPUT.value();

            if (input_entered.size() > 0)
            {
              if (input_entered == "exit" || input_entered == "bye")
              {
                main_loop_exit = true;
              }
              else
              {
                // Testing
                if (input_entered[0] == 'e')
                {
                  input_entered.erase(0, 1);
                  //sdSystem.OUTPUT_OLLAMA_RESPONSE.add_to(sdSystem.VECTORDB_SYSTEM.submit_question(input_entered), sdSystem.OUTPUT_FOCUS); 
                  sdSystem.VECTORDB_SYSTEM.submit_question(sdSystem.PROGRAM_TIME.current_frame_time(), input_entered);
                  sdSystem.OUTPUT_INPUT.clear();
                }
                else
                {
                  // submit request
                  sdSystem.OLLAMA_SYSTEM.set_status(OLLAMA_API_RESPONS_GENERATING);
                  sdSystem.OLLAMA_SYSTEM.set_request(input_entered);
                }
              }
            }
          }
        }

        sdSystem.OUTPUT_CLOCK.output(sdSystem.OUTPUT_FOCUS);
        sdSystem.OUTPUT_INPUT.output(sdSystem.OUTPUT_FOCUS);
        sdSystem.OUTPUT_OLLAMA_RESPONSE.output(sdSystem.OUTPUT_FOCUS);
      }


      // ------------------------------------------------------------------------- //
      // Sleep till next cycle check.
      
      // Make sure non of these are commented out, or the system will never sleep.
      //sdSystem.PROGRAM_TIME.request_ready_time(sdSystem.SCREENIO_SLEEP_TIMER.get_ready_time(), 'a');
      //sdSystem.PROGRAM_TIME.request_ready_time(sdSystem.OLLAMA_SLEEP_TIMER.get_ready_time(), 'b');
      //sdSystem.PROGRAM_TIME.request_ready_time(sdSystem.OLLAMA_RESPONSE_THREAD.get_ready_time(), 'c');
      //sdSystem.PROGRAM_TIME.request_ready_time(sdSystem.EMBEDDING_SLEEP_TIMER.get_ready_time(), 'd');
      sdSystem.PROGRAM_TIME.request_ready_time(sdSystem.SCREENIO_SLEEP_TIMER.get_ready_time());
      sdSystem.PROGRAM_TIME.request_ready_time(sdSystem.OLLAMA_SLEEP_TIMER.get_ready_time());
      sdSystem.PROGRAM_TIME.request_ready_time(sdSystem.OLLAMA_RESPONSE_THREAD.get_ready_time());
      sdSystem.PROGRAM_TIME.request_ready_time(sdSystem.EMBEDDING_SLEEP_TIMER.get_ready_time());

      sdSystem.PROGRAM_TIME.sleep_till_next_frame();

    } // end of main loop

    // ------------------------------------------------------------------------- //
    // ------------------------------------------------------------------------- //

    // Clean up and exit.
    sdSystem.INPUT.clear_screeen();

    // Shutdown any open threads process
    // Restore the terminal
    sdSystem.OLLAMA_RESPONSE_THREAD.wait_for_thread_to_finish();
    //sdSystem.VECTORDB_SYSTEM.wait_for_thread_to_finish();
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
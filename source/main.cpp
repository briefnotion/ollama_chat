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
  //sdSystem.OLLAMA_SYSTEM.PROPS.MODEL = "llama3.1:8b";
  sdSystem.OLLAMA_SYSTEM.PROPS.MODEL = "llama3.2:latest";
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
    int clock_clounter = 101;

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

    sdSystem.OLLAMA_SYSTEM.OLLAMA_RESPONSE_THREAD.create(OLLAMA_RESPONSE_THREAD_TIMER_DELAY);
    sdSystem.VECTORDB_SYSTEM.PYTHON_QUESTION_RESPONSE_THREAD.create(VECTORDB_API_RESPONSE_THREAD_TIMER_DELAY);

    // Send the output of the create to the screen.
    sdSystem.OUTPUT_OLLAMA_RESPONSE.output(sdSystem.OUTPUT_FOCUS);

    // ------------------------------------------------------------------------- //

    FLED_TIME_VAR time; 
    std::chrono::time_point<std::chrono::system_clock> tmeNow = std::chrono::system_clock::now();
    std::chrono::duration<double>  dur = tmeNow.time_since_epoch();

    time.put_seconds(dur.count());

    // ------------------------------------------------------------------------- //

    // Main Thread Loop
    while (main_loop_exit == false)
    {
      // ------------------------------------------------------------------------- //
      {
        // Main loop set up and start
        if (sdSystem.PROGRAM_TIME.setframetime() == true)
        {
          sdSystem.OUTPUT_OLLAMA_RESPONSE.add_to("CLOCK SKEW DETECTED\n", sdSystem.OUTPUT_FOCUS);
          sdSystem.PROGRAM_TIME.clear_error();
        }

        // Threading

        // Close all completed and active threads after sleep cycle is complete.
        sdSystem.OLLAMA_SYSTEM.OLLAMA_RESPONSE_THREAD.check_for_completition();
        sdSystem.VECTORDB_SYSTEM.PYTHON_QUESTION_RESPONSE_THREAD.check_for_completition();
      }

      // ------------------------------------------------------------------------- //

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

          tmeNow = std::chrono::system_clock::now();
          dur = tmeNow.time_since_epoch();

          time.put_seconds(dur.count());

          sdSystem.OUTPUT_CLOCK.clear();
          sdSystem.OUTPUT_CLOCK.redraw();
          sdSystem.OUTPUT_CLOCK.add_to(linemerge_left_justify("---------------------------------", 
                                        "(" + clock + ") " +
                                        "(" + to_string(sdSystem.OLLAMA_SYSTEM.get_status()) + ") " + 
                                        "(" + to_string(sdSystem.VECTORDB_SYSTEM.get_status()) + ") " + 
                                        
                                        /*
                                        "(" + to_string(sdSystem.PROGRAM_TIME.current_frame_time()) + ") " + 
                                        */

                                        "(" + to_string(time.get_year()) + "-" + 
                                              to_string(time.get_month()) + "-" + 
                                              to_string(time.get_day()) + " " + 
                                              to_string(time.get_hour()) + ":" + 
                                              to_string(time.get_minute()) + ":" + 
                                              to_string(time.get_second()) + ") " + 
                                        
                                        " INPUT:"), sdSystem.OUTPUT_FOCUS);
        }
      }

      // ------------------------------------------------------------------------- //
      // Ollama check routine

      //  Never comment this out or the system will never sleep
      if (sdSystem.OLLAMA_SLEEP_TIMER.is_ready(sdSystem.PROGRAM_TIME.current_frame_time()) == true)
      {
        sdSystem.OLLAMA_SYSTEM.process(sdSystem.OUTPUT_OLLAMA_RESPONSE, sdSystem.OUTPUT_FOCUS);
      }

      // ------------------------------------------------------------------------- //
      // Embedding check routine

      //  Never comment this out or the system will never sleep
      if (sdSystem.EMBEDDING_SLEEP_TIMER.is_ready(sdSystem.PROGRAM_TIME.current_frame_time()) == true)
      {
        sdSystem.VECTORDB_SYSTEM.process(sdSystem.OUTPUT_OLLAMA_RESPONSE, sdSystem.OUTPUT_FOCUS, sdSystem.OLLAMA_SYSTEM);
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

            sdSystem.OUTPUT_OLLAMA_RESPONSE.add_to("ENTERED: " + input_entered, sdSystem.OUTPUT_FOCUS);
            sdSystem.OUTPUT_OLLAMA_RESPONSE.add_to("\n\n-----\n", sdSystem.OUTPUT_FOCUS);

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
                  sdSystem.VECTORDB_SYSTEM.submit_question(input_entered);
                  sdSystem.OUTPUT_INPUT.clear();
                }
                else if (input_entered[0] == 'm')
                {
                  input_entered.erase(0, 1);
                  sdSystem.VECTORDB_SYSTEM.submit_file_to_embed(input_entered);
                  sdSystem.OUTPUT_INPUT.clear();
                }
                else if (input_entered[0] == 'b')
                {
                  input_entered.erase(0, 1);
                  sdSystem.VECTORDB_SYSTEM.submit_clear_database();
                  sdSystem.OUTPUT_INPUT.clear();
                }
                else if (input_entered[0] == 'd')
                {
                  input_entered.erase(0, 1);
                  sdSystem.VECTORDB_SYSTEM.submit_list_database();
                  sdSystem.OUTPUT_INPUT.clear();
                }
                else if (input_entered[0] == 'i')
                {
                  input_entered.erase(0, 1);
                  sdSystem.VECTORDB_SYSTEM.submit_question_to_ollama(input_entered, "i");
                  sdSystem.OUTPUT_INPUT.clear();
                }
                else if (input_entered[0] == 'n')
                {
                  input_entered.erase(0, 1);
                  sdSystem.VECTORDB_SYSTEM.submit_question_to_ollama_par(input_entered, "n", sdSystem.OLLAMA_SYSTEM);
                  sdSystem.OUTPUT_INPUT.clear();
                }
                else
                {
                  // submit request
                  sdSystem.OLLAMA_SYSTEM.submit_question(input_entered);
                  sdSystem.OUTPUT_INPUT.clear();
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
      sdSystem.PROGRAM_TIME.request_ready_time(sdSystem.EMBEDDING_SLEEP_TIMER.get_ready_time());

      // Not necessary at this time because pacing is handled by mutex.  If implemented be sure to 
      //  call thread.check_to_run_routine_on_thread(double Time_Frame)
      //sdSystem.PROGRAM_TIME.request_ready_time(sdSystem.OLLAMA_SYSTEM.OLLAMA_RESPONSE_THREAD.get_ready_time());
      //sdSystem.PROGRAM_TIME.request_ready_time(sdSystem.VECTORDB_SYSTEM.PYTHON_QUESTION_RESPONSE_THREAD.get_ready_time());

      sdSystem.PROGRAM_TIME.sleep_till_next_frame();

    } // end of main loop

    // ------------------------------------------------------------------------- //
    // ------------------------------------------------------------------------- //

    // Clean up and exit.
    sdSystem.INPUT.clear_screeen();

    // Shutdown any open threads process
    // Restore the terminal
    sdSystem.OLLAMA_SYSTEM.OLLAMA_RESPONSE_THREAD.wait_for_thread_to_finish("OLLAMA_RESPONSE_THREAD");
    sdSystem.VECTORDB_SYSTEM.PYTHON_QUESTION_RESPONSE_THREAD.wait_for_thread_to_finish("PYTHON_QUESTION_RESPONSE_THREAD");
    
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
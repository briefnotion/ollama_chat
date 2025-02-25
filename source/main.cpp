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

  // Thoughts System
  THOUGHTS THOUGHTS_SYSTEM;
  
  bool main_loop_exit = false;
  bool generate_closing = false;
  bool get_opening = true;

  // ------------------------------------------------------------------------- //
  {
    // Testing

    /*
    int rows = 0;
    int cols = 0;
    get_console_size(rows, cols);

    cout << "Screen Size " << rows << ":" << cols << endl;
    */

    int ret_code = 0;
    
    // Testing code at start. Return 1 if fail.
    /*
      TEST CODE HERE
    */

    if (ret_code == 1)
    {
      exit(ret_code);
    }
  }

  // ------------------------------------------------------------------------- //
  // Initialize Timers 
  sdSystem.SCREENIO_SLEEP_TIMER.set( 1000 / SCREENIO_SLEEP_TIMER_DELAY );
  sdSystem.OLLAMA_SLEEP_TIMER.set( 1000 / OLLAMA_SLEEP_TIMER_DELAY );
  sdSystem.EMBEDDING_SLEEP_TIMER.set( 1000 / EMBEDDING_SLEEP_TIMER_DELAY );
  sdSystem.THOUGHTS_SLEEP_TIMER.set( 1000 / THOUGHTS_SLEEP_TIMER_DELAY );

  sdSystem.PROGRAM_TIME.create();    //  Get current time.  This will be our timeframe to work in.
  if (sdSystem.PROGRAM_TIME.setframetime() == true)
  {
    sdSystem.PROGRAM_TIME.clear_error();
  }

  {
    // Only run main look if connection to server is sucessful.
    THOUGHTS_SYSTEM.OLLAMA_SYSTEM.PROPS.MODEL = "llama3.2:latest";
    THOUGHTS_SYSTEM.OLLAMA_SYSTEM.PROPS.URL = "http://localhost:11434";
  }

  // ------------------------------------------------------------------------- //
  // Print Revision
  sdSystem.OUTPUT_OLLAMA_RESPONSE.add_to(version_info, sdSystem.OUTPUT_FOCUS);

  // ------------------------------------------------------------------------- //
  // Properties Setup
  {
    // Prepair Variables:
    sdSystem.INPUT.create();

    // Clock, and other things, masking as the input title.
    sdSystem.OUTPUT_CLOCK.PROPS.POSITION_X = 0;
    sdSystem.OUTPUT_CLOCK.PROPS.POSITION_Y = 0;
    sdSystem.OUTPUT_CLOCK.PROPS.LINES = 1;
    sdSystem.OUTPUT_CLOCK.create(0);

    // Input Text Box:
    //sdSystem.OUTPUT_INPUT.PROPS.TITLE = "  INPUT";
    sdSystem.OUTPUT_INPUT.PROPS.POSITION_X = 0;
    sdSystem.OUTPUT_INPUT.PROPS.POSITION_Y = 1;
    sdSystem.OUTPUT_INPUT.PROPS.LINES = 2;
    sdSystem.OUTPUT_INPUT.create(1);

    // Output Text Box:
    sdSystem.OUTPUT_OLLAMA_RESPONSE.PROPS.TITLE = "  OUTPUT";
    sdSystem.OUTPUT_OLLAMA_RESPONSE.PROPS.POSITION_X = 0;
    sdSystem.OUTPUT_OLLAMA_RESPONSE.PROPS.POSITION_Y = 3;
    sdSystem.OUTPUT_OLLAMA_RESPONSE.PROPS.LINES = 100;
    sdSystem.OUTPUT_OLLAMA_RESPONSE.PROPS.RECORD_HISTORY = true;
    sdSystem.OUTPUT_OLLAMA_RESPONSE.create(2);

    // Memory Processing Module
    THOUGHTS_SYSTEM.THOUGHT_CONTOL.MEMORY.PROPS.WORKING_DIRECTORY = "~/chat_api/";
    THOUGHTS_SYSTEM.THOUGHT_CONTOL.MEMORY.PROPS.DIR_CHAT_UNPROCESSED = "history_unprocessed/";
    THOUGHTS_SYSTEM.THOUGHT_CONTOL.MEMORY.PROPS.DIR_CHAT_PROCESSED = "history_processed/";
    THOUGHTS_SYSTEM.THOUGHT_CONTOL.MEMORY.PROPS.DIR_MEMORY_FILES = "memory_files/";

    THOUGHTS_SYSTEM.create();

    // Blank the screen.
    sdSystem.INPUT.clear_screeen();
  }

  // ------------------------------------------------------------------------- //

  // Send the output of the create to the screen.
  sdSystem.OUTPUT_OLLAMA_RESPONSE.output(sdSystem.OUTPUT_FOCUS);

  // ------------------------------------------------------------------------- //

  SIMPLE_MAIN_LOOP_PROCESSOR_USAGE processor;
  string processor_status_display_simple_old = "";
  string processor_status_display_simple = "";

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
      THOUGHTS_SYSTEM.OLLAMA_SYSTEM.OLLAMA_RESPONSE_THREAD.check_for_completition();
      THOUGHTS_SYSTEM.THOUGHT_CONTOL.VECTORDB_SYSTEM.PYTHON_QUESTION_RESPONSE_THREAD.check_for_completition();
    }

    // ------------------------------------------------------------------------- //

    processor_status_display_simple = "";

    if (THOUGHTS_SYSTEM.OLLAMA_SYSTEM.get_status() > 0)
    {
      // Ollama System
      processor_status_display_simple +=  " (olama status:" + to_string(THOUGHTS_SYSTEM.OLLAMA_SYSTEM.get_status()) + ")";
    }
    if (THOUGHTS_SYSTEM.THOUGHT_CONTOL.VECTORDB_SYSTEM.get_status() > 0)
    {
      // VectorDB System
      processor_status_display_simple += " (database status:" + to_string(THOUGHTS_SYSTEM.THOUGHT_CONTOL.VECTORDB_SYSTEM.get_status()) + ")";
    }
    if (THOUGHTS_SYSTEM.thought_count() > 0)
    {
      // Thought Count, and Current Thought, and Thought Stage
      processor_status_display_simple += " (thought count:" + to_string(THOUGHTS_SYSTEM.thought_count()) + 
                                                " \"" + THOUGHTS_SYSTEM.thought_current() + 
                                                "\" stage:" + to_string(THOUGHTS_SYSTEM.thought_stage());
      if (THOUGHTS_SYSTEM.isolated())
      {
        processor_status_display_simple += " isolated ";
      }

      processor_status_display_simple += ")";
    }
    
    if (processor.changed() || processor_status_display_simple_old != processor_status_display_simple)
    {
      processor_status_display_simple_old = processor_status_display_simple;
      
      sdSystem.OUTPUT_CLOCK.clear();
      sdSystem.OUTPUT_CLOCK.redraw();
      sdSystem.OUTPUT_CLOCK.add_to(linemerge_left_justify("-------------------------------------------------------------------------", 
                                    processor.what_is_it() +
                                    processor_status_display_simple +
                                    " INPUT:"), sdSystem.OUTPUT_FOCUS);
    }

    // ------------------------------------------------------------------------- //
    // Opening and Closing routines

    if (get_opening)
    {
      get_opening = false;
      THOUGHTS_SYSTEM.interact_input("", false, "opening");
    }

    if (generate_closing)
    {
      if (THOUGHTS_SYSTEM.THOUGHT_CONTOL.MEMORY.FILE_MANAGER.is_file_ready("conversation_closing_previous"))
      {
        main_loop_exit = true;
      }
    }

    // ------------------------------------------------------------------------- //
    // Ollama check routine

    //  Never comment this out or the system will never sleep
    if (sdSystem.OLLAMA_SLEEP_TIMER.is_ready(sdSystem.PROGRAM_TIME.current_frame_time()) == true)
    {
      THOUGHTS_SYSTEM.OLLAMA_SYSTEM.check();
    }

    // ------------------------------------------------------------------------- //
    // Embedding check routine

    //  Never comment this out or the system will never sleep
    if (sdSystem.EMBEDDING_SLEEP_TIMER.is_ready(sdSystem.PROGRAM_TIME.current_frame_time()) == true)
    {
      //sdSystem.VECTORDB_SYSTEM.process(sdSystem.OUTPUT_OLLAMA_RESPONSE, sdSystem.OUTPUT_FOCUS, sdSystem.OLLAMA_SYSTEM);
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

          sdSystem.OUTPUT_OLLAMA_RESPONSE.add_to("YOU:\n     " + input_entered, sdSystem.OUTPUT_FOCUS);
          sdSystem.OUTPUT_OLLAMA_RESPONSE.seperater(sdSystem.OUTPUT_FOCUS);

          if (input_entered.size() > 0)
          {
            if (input_entered == "bye")
            {
              // turn off the recorder to save the conclusion
              sdSystem.OUTPUT_OLLAMA_RESPONSE.PROPS.RECORD_HISTORY = false;

              sdSystem.OUTPUT_OLLAMA_RESPONSE.add_to("   *---- EXITING THE SYSTEM\n", sdSystem.OUTPUT_FOCUS);
              sdSystem.OUTPUT_OLLAMA_RESPONSE.add_to("   *---- GENERATING CLOSING\n", sdSystem.OUTPUT_FOCUS);

              string conclusion_request = 
                //"i need to remember this entire conversation. write a detailed explaination of everything we talked about. feel free tell me as much as you possibly can and put more detail in the most important things.";
                //"No need to be polite. Write a detailed factual explaination of everything we talked about. Feel free tell me as much as you possibly can and put more detail in the most important things. Disregard any mentions of, not having a previous conversation";
                //"No need to be polite. Write a detailed description of everything we talked about. Feel free tell me as much as you possibly can and prioritize the most discussed.";
                "No need to be polite. Write a detailed description of everything we talked about. Feel free tell me as much as you possibly can and start with the most recent discussion and work your way back.";

              THOUGHTS_SYSTEM.interact_input(conclusion_request, false, "in conclusion");
              generate_closing = true;
            }
            else if (input_entered == "FORCEEXIT")
            {
              main_loop_exit = true;
            }
            else
            {
              THOUGHTS_SYSTEM.interact_input(input_entered);
            }
          }
        }
      }

      sdSystem.OUTPUT_CLOCK.output(sdSystem.OUTPUT_FOCUS);
      sdSystem.OUTPUT_INPUT.output(sdSystem.OUTPUT_FOCUS);
      sdSystem.OUTPUT_OLLAMA_RESPONSE.output(sdSystem.OUTPUT_FOCUS);
    }

    // ------------------------------------------------------------------------- //
    // Thoughts check routine
    if (sdSystem.THOUGHTS_SLEEP_TIMER.is_ready(sdSystem.PROGRAM_TIME.current_frame_time()) == true)
    {
      THOUGHTS_SYSTEM.process(sdSystem);
    }

    // ------------------------------------------------------------------------- //
    // Sleep till next cycle check.
    
    // Make sure non of these are commented out, or the system will never sleep.
    sdSystem.PROGRAM_TIME.request_ready_time(sdSystem.SCREENIO_SLEEP_TIMER.get_ready_time());
    sdSystem.PROGRAM_TIME.request_ready_time(sdSystem.OLLAMA_SLEEP_TIMER.get_ready_time());
    sdSystem.PROGRAM_TIME.request_ready_time(sdSystem.EMBEDDING_SLEEP_TIMER.get_ready_time());
    sdSystem.PROGRAM_TIME.request_ready_time(sdSystem.THOUGHTS_SLEEP_TIMER.get_ready_time());

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

  // Save Closing
  THOUGHTS_SYSTEM.THOUGHT_CONTOL.MEMORY.save_memory_files();

  // Remember
  THOUGHTS_SYSTEM.THOUGHT_CONTOL.MEMORY.save_chat_history(sdSystem.OUTPUT_OLLAMA_RESPONSE.HISTORY);

  // Shutdown any open threads process
  THOUGHTS_SYSTEM.OLLAMA_SYSTEM.OLLAMA_RESPONSE_THREAD.wait_for_thread_to_finish("OLLAMA_RESPONSE_THREAD");
  THOUGHTS_SYSTEM.THOUGHT_CONTOL.VECTORDB_SYSTEM.PYTHON_QUESTION_RESPONSE_THREAD.wait_for_thread_to_finish("PYTHON_QUESTION_RESPONSE_THREAD");
  
  // Restore the terminal
  sdSystem.INPUT.restore_terminal_settings();

  return 0;
}

// ------------------------------------------------------------------------- //

#endif // MAIN_CPP
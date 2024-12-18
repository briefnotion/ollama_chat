#ifndef REMEMBER_CPP
#define REMEMBER_CPP

#include "remember.h"

using namespace std;
// ------------------------------------------------------------------------- //

void REMEMBER::save_chat_history(deque<string> &chat_history)
{
  string directory_name_unprocessed = "history_unprocessed/";
  string directory_name_processed =   "history_processed/";

  string filename = "chat_history." + file_format_system_time() + ".txt";
  
  // ---

  string save_filename = PROPS.WORKING_DIRECTORY + directory_name_unprocessed + filename;

  // ---

  cout << "Chat history deque size: " << chat_history.size() << endl;
  cout << "Saving chat history to file... " << save_filename << endl;

  if (deque_string_to_file(save_filename, chat_history, true))
  {
    cout << "  Chat history saved successfully." << endl;
  }
  else
  {
    cout << "  Failed to save chat history." << endl;
  }
}

// ------------------------------------------------------------------------- //

#endif // REMEMBER_CPP
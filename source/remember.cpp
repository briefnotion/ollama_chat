#ifndef REMEMBER_CPP
#define REMEMBER_CPP

#include "remember.h"

using namespace std;

// ------------------------------------------------------------------------- //

int MEMORY_FILE_MANAGER::find_file(string Title)
{
  int ret_found_position = -1;

  if (FILES.size() > 0)
  {
    for (int pos = 0; pos < (int)FILES.size() && ret_found_position == -1; ++pos) 
    {
      if (FILES[pos].TITLE == Title) 
      {
        ret_found_position = pos;
      }
    }
  }

  return ret_found_position;
}

bool MEMORY_FILE_MANAGER::is_file_ready(string Title)
{
  bool ret_ready_status = false;
  int pos = find_file(Title);
  if (pos != -1) 
  {
    ret_ready_status = FILES[pos].IS_READY;
  }
  return ret_ready_status;
}

string MEMORY_FILE_MANAGER::get_file(string Title)
{
  string ret_file_content = "";
  int pos = find_file(Title);
  if (pos != -1) 
  {
    ret_file_content = FILES[pos].CONTENT;
  }
  else
  {
    ret_file_content = "Could not find the memory file \"" + Title + "\"\n";
  }
  return ret_file_content;
}

void MEMORY_FILE_MANAGER::add_file(MEMORY_FILE File)
{
  int pos = find_file(File.TITLE);
  if (pos == -1) 
  {
    FILES.push_back(File);
  }
  else
  {
    FILES[pos] = File;
  }
}

/*
void MEMORY_FILE_MANAGER::remove_file(string Title)
{
  // Currently, unimplemented. This is not necessary at this time.
}
*/

string MEMORY_FILE_MANAGER::memory_file_list()
{
  string ret_list = "";

  if (FILES.size() > 0)
  {
    for (int pos = 0; pos < (int)FILES.size(); pos++)
    {
      ret_list += FILES[pos].TITLE + " ";
    }
  }
  else
  {
    ret_list = "The memory file list is empty.";
  }

  return ret_list;
}

string MEMORY_FILE_MANAGER::load_all_memory_files(string Directory)
{
  FALSE_CATCH ret_success;
  vector<string> file_list;
  string output = "";

  string dir = "";
  string filename = "";
  string ext = "";

  string file_contents = "";

  output += "Loading all memory files from: " + Directory + "\n";

  if (get_files_list(Directory, file_list, ".txt"))
  {
    if (file_list.size() > 0)
    {
      for (int pos = 0; pos < (int)file_list.size(); pos++)
      {
        output += "  Loading file: " + file_list[pos] + "\n";

        split_file_path(file_list[pos], dir, filename, ext);
        MEMORY_FILE tmp_memory_file;

        tmp_memory_file.TITLE = filename;
        tmp_memory_file.CONTENT = file_to_string(file_list[pos]);
        tmp_memory_file.IS_READY = false;

        add_file(tmp_memory_file);
      }
    }
  }
  else
  {
    output += "  An error was encountered when loading memory files from: " + Directory + "\n";
  }

  return output;
}

bool MEMORY_FILE_MANAGER::save_all_memory_files(string Directory)
{
  FALSE_CATCH ret_success;

  cout << "Saving memory files ... " << endl;

  string save_filename = ""; //  PROPS.WORKING_DIRECTORY + PROPS.DIR_MEMORY_FILES + filename;

  if (FILES.size() > 0)
  {
    for (int pos = 0; pos < (int)FILES.size(); pos++)
    {
      save_filename = Directory + FILES[pos].TITLE + ".txt";

      if (string_to_file(save_filename, FILES[pos].CONTENT, false))
      {
        cout << "  File saved successfully: " << save_filename << endl;
        ret_success.catch_false(true);
      }
      else
      {
        cout  << endl << "  File failed to saved: " << save_filename << endl;
        ret_success.catch_false(false);
      }
    }
  }
  else
  {
    cout << "  No memory files to save" << endl;
  }

  return !ret_success.has_false();
}

// ------------------------------------------------------------------------- //

void REMEMBER::save_chat_history(vector<string> &chat_history)
{
  string filename = "chat_history." + file_format_system_time() + ".txt";
  
  // ---

  string save_filename = PROPS.WORKING_DIRECTORY + PROPS.DIR_CHAT_UNPROCESSED + filename;

  // ---

  cout << "Chat history vector size: " << chat_history.size() << endl;
  cout << "Saving chat history to file... " << save_filename << endl;

  if (vector_string_to_file(save_filename, chat_history, true))
  {
    cout << "  Chat history saved successfully" << endl;
  }
  else
  {
    cout << "\n  Failed to save chat history\n" << endl;
  }
}

string REMEMBER::load_memory_files()
{
  return FILE_MANAGER.load_all_memory_files(PROPS.WORKING_DIRECTORY + PROPS.DIR_MEMORY_FILES);
}

void REMEMBER::save_memory_files()
{
  FILE_MANAGER.save_all_memory_files(PROPS.WORKING_DIRECTORY + PROPS.DIR_MEMORY_FILES);
}

// ------------------------------------------------------------------------- //

#endif // REMEMBER_CPP
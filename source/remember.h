#ifndef REMEMBER_H
#define REMEMBER_H

#include <iostream>
#include <string>
#include <deque>

//#include "threading.h"
#include "helper.h"
#include "rasapi.h"

using namespace std;

// ------------------------------------------------------------------------- //

class MEMORY_FILE
{
  private:

  public:
  string TITLE = "";
  string CONTENT = "";
  bool IS_READY = false;
};

class MEMORY_FILE_MANAGER
{
  vector<MEMORY_FILE> FILES;

  int find_file(string Title);      // returns pos of file in vector array.

  public:

  bool is_file_ready(string Title); // returns false if not found or false
  string get_file(string Title);    // searchs and returns CONTENT
  void add_file(MEMORY_FILE File);  // adds or modifies a file
  //void remove_file(string Title);   // unimplemented.

  string memory_file_list();  // returns a list of all files in the vector array.
                              // All files are in quotes seperated by a space.

  string load_all_memory_files(string Directory);   // loads all memory files from disk.
  bool save_all_memory_files(string Directory);     // saves all memory files to disk.
};

// ------------------------------------------------------------------------- //
class REMEMBER_PROPERTIES
{
  public:
  string WORKING_DIRECTORY = "";
  string DIR_CHAT_UNPROCESSED = "";
  string DIR_CHAT_PROCESSED = "";
  string DIR_MEMORY_FILES = "";
};

class REMEMBER
{
  private:

  public:

  REMEMBER_PROPERTIES PROPS;
  MEMORY_FILE_MANAGER FILE_MANAGER;

  void save_chat_history(vector<string> &chat_history);

  string load_memory_files();
  void save_memory_files();
};

// ------------------------------------------------------------------------- //
#endif // REMEMBER_H
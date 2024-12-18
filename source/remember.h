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
class REMEMBER_PROPERTIES
{
  public:
  string WORKING_DIRECTORY = "";
};

class REMEMBER
{
  private:

  public:

  REMEMBER_PROPERTIES PROPS;
  void save_chat_history(deque<string> &chat_history);

};

// ------------------------------------------------------------------------- //
#endif // REMEMBER_H
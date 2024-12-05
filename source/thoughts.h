#ifndef THOUGHTS_H
#define THOUGHTS_H

//#include <future>
//#include <iostream>

//#include "fled_time.h"

#include "system.h"

#include "ollama_api.h"
#include "vectordb_python_api.h"

using namespace std;

// ------------------------------------------------------------------------- //

class THOUGHTS
{
  private:

  string INPUT = "";

  bool CHANGED = false;

  public:

  void input(string Input);

  void process(SYSTEM &System);
};

// ------------------------------------------------------------------------- //

#endif // THOUGHTS_H
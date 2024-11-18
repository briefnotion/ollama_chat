#ifndef VECTORDB_PYTHON_API_H
#define VECTORDB_PYTHON_API_H

#include <iostream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>

#include "ollama_api.h"

using namespace std;

// ------------------------------------------------------------------------- //

class VECTORDB_PYTHON_API
{
  private:

  string callPythonScript();

  string exec(const char* cmd);

  public:

  int embedding_test_python_1();

  int embedding_test_python_2();

  int embedding_test_python_3();

};

// ------------------------------------------------------------------------- //

#endif // VECTORDB_PYTHON_API_H
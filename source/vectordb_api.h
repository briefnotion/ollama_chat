#ifndef VECTORDB_API_H
#define VECTORDB_API_H

#include <iostream>
//#include <string>
//#include <vector>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>

#include "ollama_api.h"

#include <nlohmann/json.hpp>
//#include "httplib.h"

//#include "ChromaDB/ChromaDB.h"

//#include "cJSON.h"
//#include <curl/curl.h>

//#include "helper.h"
//#include "ollama_helper.h"
//#include "tty_io.h"


using namespace std;

// ------------------------------------------------------------------------- //

class VECTORDB_API
{
  public:
  
  int embedding_test_1();
  
  int embedding_test_2();

  int embedding_test_3();

  //---

  //int embedding_test_cjson_1();

  //---

  string callPythonScript();

  int embedding_test_python_1();

  string exec(const char* cmd);

  int embedding_test_python_2();

  int embedding_test_python_3();

};

// ------------------------------------------------------------------------- //

#endif // VECTORDB_API_H
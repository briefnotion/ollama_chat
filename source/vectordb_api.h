#ifndef VECTORDB_API_H
#define VECTORDB_API_H

#include <iostream>
//#include <string>
//#include <vector>

#include "ollama_api.h"

#include <nlohmann/json.hpp>
#include "httplib.h"

#include "ChromaDB/ChromaDB.h"

//#include "helper.h"
//#include "ollama_helper.h"
//#include "tty_io.h"



using namespace std;

// ------------------------------------------------------------------------- //

class VECTORDB_API
{
  public:
  
  int embedding_test_0();
  
  int embedding_test_1();

  int embedding_test_2();

};


// ------------------------------------------------------------------------- //

#endif // VECTORDB_API_H
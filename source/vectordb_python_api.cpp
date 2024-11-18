#ifndef VECTORDB_PYTHON_API_CPP
#define VECTORDB_PYTHON_API_CPP


#include "vectordb_python_api.h"

using namespace std;

// ------------------------------------------------------------------------- //
string VECTORDB_PYTHON_API::callPythonScript() 
{
  // Call the Python script
  system("python3 ../python/script.py");

  // Read the result from the file
  ifstream file("output.txt");
  string result;

  if (file.is_open()) {
    getline(file, result);
    file.close();
  } else {
    result = "Error: unable to open file";
  }

  return result;
}

string VECTORDB_PYTHON_API::exec(const char* cmd) 
{
  array<char, 128> buffer;
  string result;
  unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
  if (!pipe) 
  {
    throw runtime_error("popen() failed!");
  }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) 
  {
    result += buffer.data();
  }
  return result;
}

int VECTORDB_PYTHON_API::embedding_test_python_1()
{
  string result = callPythonScript();
  cout << "Result from Python script: " << result << endl;

  return 1;
}

int VECTORDB_PYTHON_API::embedding_test_python_2() 
{
  string command = "python3 ../python/script.py";
  string result = exec(command.c_str());
  cout << "Result from Python script: " << result << endl;
    
  return 1;
}

int VECTORDB_PYTHON_API::embedding_test_python_3() 
{
  string andand = "&& ";

  string bash = "bash -c '";

  string environment = "source /home/briefn/py/venv/bin/activate ";
  string script = "python3 ../python/search.py ";
  string question = "what happened in tiawan?";

  string enviornement_stop = "deactivate";

  string command = environment + andand + script + question;
  //string command = script + question;
  //string command = environment;

  string bcommand = bash + command + "'";

  cout << bcommand << endl;

  string result = exec(bcommand.c_str());
  cout << "Result from Python script: " << result << endl;
    
  return 1;
}


// ------------------------------------------------------------------------- //

#endif // VECTORDB_PYTHON_API_CPP
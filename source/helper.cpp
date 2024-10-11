#ifndef HELPER_CPP
#define HELPER_CPP

#include "helper.h"

using namespace std;

void print_vector_with_title(string Title, vector<string> &Vector_String)
{
  if (Vector_String.size() > 0)
  {
    cout << Title << endl;

    for (int i = 0 ; i < (int)Vector_String.size(); ++i)
    {
      cout << "  " << Vector_String[i] << endl;
    }

    cout << endl;
  
  }
}

#endif //HELPER_CPP
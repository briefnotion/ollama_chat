#ifndef THREADING_CPP
#define THREADING_CPP

#include "threading.h"

using namespace std;

// ------------------------------------------------------------------------- //

void THREADING_INFO::create(int Time_in_FPS)
{
  THREAD_TIMER.set( 1000 / Time_in_FPS );
}

unsigned long THREADING_INFO::get_ready_time()
{
  return THREAD_TIMER.get_ready_time();
}

bool THREADING_INFO::is_ready(unsigned long Time_Frame)
{
  if (THREAD_TIMER.is_ready(Time_Frame) == true)
  {
    return true;
  }
  else
  {
    return false;
  }
}

// ------------------------------------------------------------------------- //

#endif //THREADING_CPP
#ifndef THREADING_H
#define THREADING_H

#include "fled_time.h"

using namespace std;

// ------------------------------------------------------------------------- //

class THREADING_INFO
{
  private:

  TIMED_IS_READY  THREAD_TIMER;
  
  public:

  bool IS_RUNNING = false;

  void create(int Time_in_FPS);

  unsigned long get_ready_time();

  bool is_ready(unsigned long Time_Frame);
};

// ------------------------------------------------------------------------- //

#endif //THREADING_H
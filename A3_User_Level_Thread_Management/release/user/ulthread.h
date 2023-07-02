#ifndef __UTHREAD_H__
#define __UTHREAD_H__

#include <stdbool.h>

#define MAXULTHREADS 100

enum ulthread_state {
  FREE,
  RUNNABLE,
  YIELD,
};

enum ulthread_scheduling_algorithm {
  ROUNDROBIN,   
  PRIORITY,     
  FCFS,         // first-come-first serve
};

// Saved registers for kernel context switches.
struct ulthread_context {
  uint64 ra; // start
  uint64 sp; // stack pointer

  // callee-saved ... taking 6 args from test cases.
  uint64 arg0;
  uint64 arg1;
  uint64 arg2;
  uint64 arg3;
  uint64 arg4;
  uint64 arg5;
};

struct ulthread {
  int tid; // ul thread id
  enum ulthread_state state; // ul thread state
  struct ulthread_context ult_context; // ul thread context
  enum ulthread_scheduling_algorithm sched_algo; // ul thread scheduling algorithm
  int priority; // ul thread priority
  uint64 c_time; // ul thread clocking time information
};

#endif
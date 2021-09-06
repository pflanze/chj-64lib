/*
  Copyright (C) 2021 Christian Jaeger, <ch@christianjaeger.ch>
  Published under the terms of the MIT License, see the LICENSE file.
*/
/*

  Triggering the VICE monitor/debugger:

  Start VICE via `x64 -initbreak 0x02FF ...`, call init_break() at
  start time, then BREAK() when you want to trigger the debugger.

  If you start VICE without this option, calling the break/trap will
  have no effect.


  To use from assembly, `jsr break`.

*/

void debug_init();
void break_enable(); // included in debug_init()
void break_disable();

// some might call this TRAP instead

#ifdef __C64__

# define BREAK() asm("jsr break")

#else
// assuming POSIX

#include <sys/types.h>
#include <signal.h>

#define BREAK() raise(SIGTRAP)

#endif

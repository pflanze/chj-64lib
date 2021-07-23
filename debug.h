/*

  Triggering the VICE monitor/debugger:

  Start VICE via `x64 -initbreak 0x02FF ...`, call init_break() at
  start time, then BREAK() when you want to trigger the debugger.

  If you start VICE without this option, calling the break/trap will
  have no effect.
  
 */

void debug_init();
void break_enable(); // included in debug_init()
void break_disable();

#define BREAK() asm("jsr break")
// or named as in spaceInvaders:
#define TRAP() asm("jsr break")


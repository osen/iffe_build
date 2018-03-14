#include "iffe.h"

#include <stdlib.h>

void Debugger::debug()
{
  std::string cmd = "gdb -q " + state.exePath;

  if(system(cmd.c_str()) != 0)
  {
    throw Exception("Failed to initialize debugger");
  }
}

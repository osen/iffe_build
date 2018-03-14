#include "iffe.h"

#include <iostream>
#include <vector>

State state;

void safe_main(std::vector<std::string>& args)
{
  Application::prepareEnvironment();
  Application::processArguments(args);
  Directory::scan("assets", state.assets);
  //Resources::generate(state.assets);
  Compiler::generateObj(state);
  Compiler::link(state.assets);

  if(state.useDebugger)
  {
    Debugger::debug();
  }
}

int main(int argc, char* argv[])
{
  try
  {
    std::vector<std::string> args;

    for(size_t i = 0; i < argc; i++)
    {
      args.push_back(argv[i]);
    }

    safe_main(args);
    return 0;
  }
  catch(std::exception& e)
  {
    std::cout << "Exception: " << e.what() << std::endl;
    return 1;
  }
}

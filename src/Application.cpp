#include "iffe.h"

void Application::prepareEnvironment()
{
  state.buildType = BuildType::RELEASE;

#ifdef __OpenBSD__
  state.platform = "openbsd";
#endif

#ifdef __FreeBSD__
  state.platform = "freebsd";
#endif

#ifdef _win32
  state.platform = "windows";
#endif

  if(state.platform == "")
  {
    throw Exception("Unknown platform");
  }
}

void Application::processArguments(std::vector<std::string>& args)
{
  for(size_t i = 1; i < args.size(); i++)
  {
    if(args.at(i) == "debug")
    {
      state.buildType = BuildType::DEBUG;
      state.useDebugger = true;
    }
    else if(args.at(i) == "-g")
    {
      state.buildType = BuildType::DEBUG;
    }
    else
    {
      throw Exception("Unknown argument: " + args.at(i));
    }
  }

  std::string buildType = "release";

  if(state.buildType == BuildType::DEBUG)
  {
    buildType = "debug";
  }

  state.exePath = "build/"+state.platform+"_amd64_"+buildType+"/program";
}

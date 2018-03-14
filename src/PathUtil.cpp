#include "iffe.h"

#include <sys/stat.h>

bool PathUtil::isDirectory(std::string path)
{
  struct stat s = {0};

  if(stat(path.c_str(), &s) != 0)
  {
    throw Exception("Failed to stat: " + path);
  }

  if(s.st_mode & S_IFDIR)
  {
    return true;
  }

  return false;
}

std::string PathUtil::baseName(std::string path)
{
  std::string n = name(path);

  for(size_t i = 0; i < n.length(); i++)
  {
    if(n.at(i) == '.')
    {
      return n.substr(0, i);
    }
  }

  return n;
}

std::string PathUtil::suffix(std::string path)
{
  size_t lastSep = 0;
  std::string n = name(path);

  for(int i = n.length() - 1; i >= 0; i--)
  {
    if(n.at(i) == '.')
    {
      lastSep = i;
      break;
    }
  }

  if(lastSep == 0) return "";

  return n.substr(lastSep + 1);
}

std::string PathUtil::directory(std::string path)
{
  size_t lastSep = 0;

  for(int i = path.length() - 1; i >= 0; i--)
  {
    if(path.at(i) == '/')
    {
      lastSep = i;
      break;
    }
  }

  return path.substr(0, lastSep);
}

std::string PathUtil::name(std::string path)
{
  size_t lastSep = 0;

  for(int i = path.length() - 1; i >= 0; i--)
  {
    if(path.at(i) == '/')
    {
      lastSep = i;
      break;
    }
  }

  return path.substr(lastSep + 1);
}

time_t PathUtil::modified(std::string path)
{
  struct stat s = {0};

  if(stat(path.c_str(), &s) != 0)
  {
    throw Exception("Failed to stat: " + path);
  }

  return s.st_mtime;
}

bool PathUtil::exists(std::string path)
{
  struct stat s = {0};

  if(stat(path.c_str(), &s) != 0)
  {
    return false;
  }

  return true;
}

std::string PathUtil::objPath(std::string path)
{
  for(size_t i = 0; i < path.length(); i++)
  {
    if(path.at(i) == '/')
    {
      path.at(i) = '_';
    }
    else if(path.at(i) == '.')
    {
      path.at(i) = '_';
    }
  }

  std::string buildType = "release";

  if(state.buildType == BuildType::DEBUG)
  {
    buildType = "debug";
  }

  path = "temp/"+state.platform+"_amd64_"+buildType+"/objects/" + path;
  path += ".o";

  return path;
}


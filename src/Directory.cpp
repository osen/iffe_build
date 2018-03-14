#include "iffe.h"

#include <dirent.h>
#include <stdio.h>

#include <iostream>

void Directory::scan(std::string path, std::vector<Asset>& output)
{
  DIR* dir = opendir(path.c_str());

  if(!dir)
  {
    throw Exception("Failed to open directory: " + path);
  }

  dirent* entry = NULL;

  while((entry = readdir(dir)))
  {
    if(entry->d_name[0] == '.') continue;

    Asset a;

    a.path = path + "/" + entry->d_name;

    if(PathUtil::isDirectory(a.path))
    {
      scan(a.path, output);
    }
    else
    {
      a.baseName = PathUtil::baseName(a.path);
      a.suffix = PathUtil::suffix(a.path);
      a.name = PathUtil::name(a.path);
      a.modified = PathUtil::modified(a.path);
      a.depModified = 0;

      if(a.suffix == "cpp") a.type = AssetType::SOURCE;
      else if(a.suffix == "h") a.type = AssetType::HEADER;
      else a.type = AssetType::RESOURCE;

      //std::cout << "Path: " << a.path << std::endl;
      //std::cout << "BaseName: " << a.baseName << std::endl;
      //std::cout << "Suffix: " << a.suffix << std::endl;
      //std::cout << "Name: " << a.name << std::endl;
      //std::cout << "Modified: " << a.modified << std::endl;
      //std::cout << std::endl;

      output.push_back(a);
    }
  }

  closedir(dir);
}

void Directory::create(std::string path)
{
  std::cout << "mkdir " << path << std::endl;
  //TODO: system() is ugly.

  std::string cmd = "mkdir -p " + path;

  if(system(cmd.c_str()) != 0)
  {
    throw Exception("Failed to create directory: " + path);
  }
}

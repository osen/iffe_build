#include "iffe.h"

#include <iostream>
#include <fstream>

void Compiler::scanLastChange(std::vector<Asset>& assets, Asset& asset, time_t& output)
{
  if(asset.depModified != 0)
  {
    if(asset.depModified > output) output = asset.depModified;
    return;
  }

  asset.depModified = asset.modified;
  //if(asset.modified > output) output = asset.modified;

  std::ifstream file(asset.path.c_str());

  if(!file.is_open())
  {
    throw Exception("Failed to open source file: " + asset.path);
  }

  std::string token;

  while(!file.eof())
  {
    file >> token;

    if(token == "#")
    {
      file >> token;
      token = "#" + token;
    }

    if(token != "#include") continue;
    file >> token;

    if(token.at(0) == '"' || token.at(0) == '<')
    {
      token = token.substr(1);
    }

    if(token.at(token.length() - 1) == '"' || token.at(token.length() - 1) == '>')
    {
      token = token.substr(0, token.length() - 1);
    }

    //std::cout << "[" << token << "]" << std::endl;

    for(size_t i = 0; i < assets.size(); i++)
    {
      Asset& a = assets.at(i);
      if(a.type != AssetType::HEADER) continue;

      if(a.name == token)
      {
        scanLastChange(assets, a, asset.depModified);
      }
    }
  }

  if(asset.depModified > output)
  {
    output = asset.depModified;
  }
}

void Compiler::compile(State& state, Asset& asset)
{
  //std::cout << "Compiling: " << asset.path << std::endl;

  std::string debugFlag;

  if(state.buildType == BuildType::DEBUG)
  {
    debugFlag = "-g ";
  }

  std::string cmd = "c++ "+debugFlag+"-c -o " + asset.objPath + " " + asset.path;
  //std::cout << cmd << std::endl;
  std::string simple = "c++ "+debugFlag+"-c -o " + asset.baseName + ".o " + asset.name;
  std::cout << simple << std::endl;

  if(system(cmd.c_str()) != 0)
  {
    throw Exception("Failed to compile: " + asset.path);
  }
}

void Compiler::link(std::vector<Asset>& assets)
{
  std::string buildType = "release";
  std::string debugFlag;

  if(state.buildType == BuildType::DEBUG)
  {
    buildType = "debug";
    debugFlag = "-g ";
  }

  //std::string exePath = "build/"+state.platform+"_amd64_"+buildType+"/program";

  if(!PathUtil::exists(PathUtil::directory(state.exePath)))
  {
    Directory::create(PathUtil::directory(state.exePath));
  }

  time_t lastChange = 0;
  std::string cmd = "c++ "+debugFlag+"-o " + state.exePath + " ";
  std::string simple = "ld "+debugFlag+"-o " + PathUtil::name(state.exePath) + " ";

  for(size_t i = 0; i < assets.size(); i++)
  {
    Asset& a = assets.at(i);

    if(a.type != AssetType::SOURCE) continue;

    time_t objModified = PathUtil::modified(a.objPath);
    cmd += a.objPath + " ";
    simple += a.baseName + ".o ";

    if(objModified > lastChange)
    {
      lastChange = objModified;
    }
  }

  if(PathUtil::exists(state.exePath))
  {
    if(lastChange <= PathUtil::modified(state.exePath))
    {
      return;
    }
  }

  //std::cout << "Linking: " << state.exePath << std::endl;
  //std::cout << cmd << std::endl;
  std::cout << simple << std::endl;

  if(system(cmd.c_str()) != 0)
  {
    throw Exception("Failed to link: " + state.exePath);
  }
}

void Compiler::generateObj(State& state)
{
  for(size_t i = 0; i < state.assets.size(); i++)
  {
    Asset& a = state.assets.at(i);

    if(a.type != AssetType::SOURCE) continue;

    //std::cout << a.path << std::endl;
    a.objPath = PathUtil::objPath(a.path);
    //std::cout << a.objPath << std::endl;

    time_t lastChange = 0;

    scanLastChange(state.assets, a, lastChange);

    //std::cout << "Last changed: " << lastChange << std::endl;

    if(!PathUtil::exists(PathUtil::directory(a.objPath)))
    {
      Directory::create(PathUtil::directory(a.objPath));
    }

    if(PathUtil::exists(a.objPath))
    {
      if(lastChange <= PathUtil::modified(a.objPath))
      {
        continue;
      }
    }

    compile(state, a);
  }
}

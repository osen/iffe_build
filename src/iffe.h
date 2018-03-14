#include <string>
#include <vector>
#include <time.h>

struct State;

struct AssetType
{
  static const int SOURCE = 1;
  static const int HEADER = 3;
  static const int RESOURCE = 2;
};

struct Asset
{
  std::string baseName;
  std::string name;
  std::string path;
  std::string suffix;
  int type;
  time_t modified;
  time_t depModified;
  std::string objPath;
};

struct Application
{
  static void prepareEnvironment();
  static void processArguments(std::vector<std::string>& args);
};

struct Debugger
{
  static void debug();
};

struct Directory
{
  static void scan(std::string path, std::vector<Asset>& output);
  static void create(std::string path);
};

struct Compiler
{
  static void generateObj(State& state);
  static void scanLastChange(std::vector<Asset>& assets, Asset& asset, time_t& output);
  static void compile(State& state, Asset& asset);
  static void link(std::vector<Asset>& assets);
};

struct PathUtil
{
  static bool isDirectory(std::string path);
  static std::string directory(std::string path);
  static std::string baseName(std::string path);
  static std::string suffix(std::string path);
  static std::string name(std::string path);
  static time_t modified(std::string path);
  static bool exists(std::string path);
  static std::string objPath(std::string path);
};

class Exception : public std::exception
{
public:
  Exception(std::string message);
  virtual ~Exception() throw();
  const char* what() const throw();

private:
  std::string message;
};

struct BuildType
{
  static const int DEBUG = 0;
  static const int RELEASE = 1;
};

struct State
{
  int buildType;
  std::vector<Asset> assets;
  bool useDebugger;
  std::string platform;
  std::string exePath;
};

extern State state;

#ifndef RAPL_DEVICES
#define RAPL_DEVICES
#include <string>
#include <unordered_map>
#include <vector>
#include <map>

class RAPLDevice
{
  std::string RAPL_API_PATH = "/sys/class/powercap/intel-rapl/";
  std::string getName(std::string path);

public:
  std::vector<std::unordered_map<std::string, std::string>> devices;
  std::map<std::string, unsigned long long> getEnergy();
  RAPLDevice();
};
#endif
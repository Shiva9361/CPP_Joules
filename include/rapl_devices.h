#ifndef RAPL_DEVICES
#define RAPL_DEVICES
#include <string>
#include <unordered_map>
#include <vector>
#include <map>

#ifdef _WIN32
#define EXPOSE_DLL __declspec(dllexport)
#else
#define EXPOSE_DLL
#endif
class EXPOSE_DLL RAPLDevice
{
  std::string RAPL_API_PATH = "/sys/class/powercap/intel-rapl/";
  std::string getName(std::string path);

public:
  std::unordered_map<std::string, std::string> devices;
  std::unordered_map<std::string, std::string> max_energy_devices;
  std::map<std::string, unsigned long long> getEnergy();
  RAPLDevice();
};
#endif
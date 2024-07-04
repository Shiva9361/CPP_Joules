#ifndef NVIDIA_DEVICES
#define NVIDIA_DEVICES
#include "nvml.h"
#include <dlfcn.h>
#include <vector>
#include <map>
#include <memory>
/**
 * Custom deleter is needed
 */
struct Dlcloser
{
  void operator()(void *handle) const
  {
    if (handle)
    {
      dlclose(handle);
    }
  }
};
class NVMLDevice
{
private:
  std::vector<std::unique_ptr<nvmlDevice_t>> devices;
  std::unique_ptr<uint32_t> device_count;
  std::unique_ptr<void, Dlcloser> nvmlhandle;

public:
  NVMLDevice();
  bool usable;
  std::map<std::string, unsigned long long> getEnergy();
};
#endif
#ifndef NVIDIA_DEVICES
#define NVIDIA_DEVICES
#include "nvml/nvml.h"
#include <vector>
#include <map>
#include <memory>
class NVMLDevice
{
private:
  std::vector<std::unique_ptr<nvmlDevice_t>> devices;
  std::unique_ptr<uint32_t> device_count;

public:
  NVMLDevice();
  std::map<std::string, unsigned long long> getEnergy();
};
#endif
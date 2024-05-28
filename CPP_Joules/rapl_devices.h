#ifndef RAPL_DEVICES
#define RAPL_DEVICES
#include <string>

#include "energy_type.h"
class RAPLDevice
{
  std::string RAPL_API_PATH = "/sys/class/powercap/intel-rapl/";

public:
  EnergyType getEnergy();
};
#endif
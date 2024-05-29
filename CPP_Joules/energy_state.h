#ifndef ENERGY_STATE
#define ENERGY_STATE
#include <chrono>
#include "energy_type.h"
class EnergyState
{
public:
  std::chrono::time_point<std::chrono::system_clock> timestamp;
  EnergyType *energy_values;
  EnergyState(std::chrono::time_point<std::chrono::system_clock> timestamp, EnergyType *energy);
};

#endif
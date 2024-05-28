#ifndef ENERGY_STATE
#define ENERGY_STATE
#include <chrono>
class EnergyState
{
  std::chrono::time_point<std::chrono::system_clock> timestamp;

public:
  EnergyState(std::chrono::time_point<std::chrono::system_clock> timestamp);
};

#endif
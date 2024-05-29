#include "energy_state.h"

EnergyState::EnergyState(std::chrono::time_point<std::chrono::system_clock> timestamp, EnergyType *energy)
{
  this->energy_values = energy;
  this->timestamp = timestamp;
}

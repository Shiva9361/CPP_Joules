#include <energy_state.h>
#include <string>
EnergyState::EnergyState(std::chrono::time_point<std::chrono::system_clock> timestamp, std::map<std::string, unsigned long long> energies)
{
  this->energies = energies;
  this->timestamp = timestamp;
}

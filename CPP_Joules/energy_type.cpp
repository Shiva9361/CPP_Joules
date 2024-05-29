#include "energy_type.h"

EnergyType::EnergyType(long long package_energy, long long core_energy, long long uncore_energy)
{
  this->core_energy = core_energy;
  this->uncore_energy = uncore_energy;
  this->package_energy = package_energy;
}
EnergyType::EnergyType() {}
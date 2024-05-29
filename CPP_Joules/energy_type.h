#ifndef ENERGY_TYPE
#define ENERGY_TYPE
class EnergyType
{

public:
  long long core_energy;
  long long uncore_energy;
  long long package_energy;
  EnergyType();
  EnergyType(long long package_energy, long long core_energy, long long uncore_energy);
};
#endif
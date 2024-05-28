#include "cppJoules.h"
#include <iostream>

void EnergyTracker::start()
{
  start_time = std::chrono::system_clock::now();
  inital_energy = RAPL_device.getEnergy();
}

void EnergyTracker::stop()
{
  end_time = std::chrono::system_clock::now();
  final_energy = RAPL_device.getEnergy();
}

void EnergyTracker::calculate_energy()
{
  total_core_energy = final_energy.core_energy - inital_energy.core_energy;
}
void EnergyTracker::print_energy()
{
  std::cout << std::endl;
  std::cout << "Core " << total_core_energy << std::endl;
  std::cout << std::endl;
}
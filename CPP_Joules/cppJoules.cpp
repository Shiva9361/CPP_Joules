#include "cppJoules.h"
#include <iostream>

void EnergyTracker::start()
{
  start_time = std::chrono::system_clock::now();
  initial_energy = RAPL_device.getEnergy();
}

void EnergyTracker::stop()
{
  end_time = std::chrono::system_clock::now();
  final_energy = RAPL_device.getEnergy();
}

void EnergyTracker::calculate_energy()
{
  for (int i = 0; i < RAPL_device.devices.size(); i++)
  {
    total_package_energy.push_back(final_energy[i].energy_values->package_energy - initial_energy[i].energy_values->package_energy);
    total_core_energy.push_back(final_energy[i].energy_values->core_energy - initial_energy[i].energy_values->core_energy);
    total_uncore_energy.push_back(final_energy[i].energy_values->uncore_energy - initial_energy[i].energy_values->uncore_energy);
  }
}
void EnergyTracker::print_energy()
{
  std::cout << std::endl;
  std::cout << std::endl;
  for (int i = 0; i < total_core_energy.size(); i++)
  {
    std::cout << "Device " << i << std::endl;
    std::cout << "Time " << std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count() << " seconds" << std::endl;
    std::cout << "Package " << total_package_energy[i] << std::endl;
    std::cout << "Core " << total_core_energy[i] << std::endl;
    std::cout << "Uncore " << total_uncore_energy[i] << std::endl;
    std::cout << std::endl;
  }
}
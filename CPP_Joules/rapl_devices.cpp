#include "rapl_devices.h"
#include <fstream>
#include <iostream>
#include <filesystem>
EnergyType RAPLDevice::getEnergy()
{
  std::string path = RAPL_API_PATH + "intel-rapl:0/" + "intel-rapl:0:0/" + "energy_uj";
  std::ifstream Filehandler(path);
  if (!Filehandler.good() && std::filesystem::exists(path))
  {
    std::cout << "Run with Sudo Access";
  }
  std::string core_energy_s;
  getline(Filehandler, core_energy_s);
  long long core_energy = std::stoll(core_energy_s);
  EnergyType energy(core_energy);
  return energy;
}
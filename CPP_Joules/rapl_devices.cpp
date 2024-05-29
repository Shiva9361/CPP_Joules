#include "rapl_devices.h"
#include "rapl_exceptions.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <unordered_map>
RAPLDevice::RAPLDevice()
{
  int socket_id = 0;
  std::string path = RAPL_API_PATH + "intel-rapl:" + std::to_string(socket_id);
  while (std::filesystem::exists(path))
  {
    socket_id++;
    path = RAPL_API_PATH + "intel-rapl:" + std::to_string(socket_id);
  }
  for (int i = 0; i < socket_id; i++)
  {
    std::string temp = "intel-rapl:" + std::to_string(i);
    int inner_id = 0;
    path = RAPL_API_PATH + temp + "/" + temp + ":" + std::to_string(inner_id);
    std::unordered_map<int, std::string> device;
    device[-1] = RAPL_API_PATH + temp;
    while (std::filesystem::exists(path))
    {
      device[inner_id] = path;
      inner_id++;
      path = path = RAPL_API_PATH + temp + "/" + temp + ":" + std::to_string(inner_id);
    }
    devices.push_back(device);
  }
}

std::vector<EnergyState> RAPLDevice::getEnergy()
{
  std::vector<EnergyState> energies;
  for (auto device : devices)
  {
    long long package_energy = 0;
    long long core_energy = 0;
    long long uncore_energy = 0;

    for (auto id : device)
    {
      std::string path = id.second + "/energy_uj";
      std::ifstream Filehandler(path);

      if (!Filehandler.is_open())
      {
        throw RAPLException("RAPL access denied");
      }
      std::string energy_s;
      getline(Filehandler, energy_s);
      long long energy = std::stoll(energy_s);
      if (id.first == -1)
        package_energy = energy;
      else if (id.first == 0)
        core_energy = energy;
      else
        uncore_energy = energy;
    }
    EnergyType *energy_values = new EnergyType(package_energy, core_energy, uncore_energy);
    std::chrono::time_point<std::chrono::system_clock> timestamp = std::chrono::system_clock::now();
    EnergyState energy(timestamp, energy_values);
    energies.push_back(energy);
  }
  return energies;
}
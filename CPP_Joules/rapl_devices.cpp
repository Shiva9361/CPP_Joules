#include "rapl_devices.h"
#include "rapl_exceptions.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <memory>
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

std::map<std::string, unsigned long long> RAPLDevice::getEnergy()
{
  std::map<std::string, unsigned long long> energies;
  int device_id = 0;
  for (auto device : devices)
  {

    for (auto id : device)
    {
      std::string path = id.second + "/energy_uj";
      std::ifstream Filehandler(path);

      if (!Filehandler.is_open())
      {
        throw CPPJoulesException("RAPL access denied");
      }
      std::string energy_s;
      getline(Filehandler, energy_s);
      long long energy = std::stoll(energy_s);
      if (id.first == -1)
        energies["package_" + std::to_string(device_id)] = energy;
      else if (id.first == 0)
        energies["core_" + std::to_string(device_id)] = energy;
      else
        energies["uncore_" + std::to_string(device_id)] = energy;
    }
    device_id++;
  }
  return energies;
}
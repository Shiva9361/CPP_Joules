#include <rapl_devices.h>
#include <cppJoules_exceptions.h>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <memory>
#include <unordered_map>
RAPLDevice::RAPLDevice()
{
  /**
   * Initialization of RAPL using the powercap interface
   * All the possible domains are searched for and
   * the domains that are present are added for measurement
   * Requires read access to all energy_uj files of accessible
   * domains
   *  */
  int socket_id = 0;
  std::string path = RAPL_API_PATH + "intel-rapl:" + std::to_string(socket_id);

  while (std::filesystem::exists(path))
  {
    socket_id++;
    path = RAPL_API_PATH + "intel-rapl:" + std::to_string(socket_id);
  }

  for (uint32_t i = 0; i < socket_id; i++)
  {
    std::string temp = "intel-rapl:" + std::to_string(i);
    int inner_id = 0;
    path = RAPL_API_PATH + temp + "/" + temp + ":" + std::to_string(inner_id);

    std::unordered_map<std::string, std::string> device, max_energy_device;

    devices[getName(RAPL_API_PATH + temp)] = RAPL_API_PATH + temp + "/energy_uj";
    max_energy_devices[getName(RAPL_API_PATH + temp)] = RAPL_API_PATH + temp + "/max_energy_range_uj";

    while (std::filesystem::exists(path))
    {
      devices[getName(path) + "-" + std::to_string(i)] = path + "/energy_uj";
      max_energy_device[getName(path) + "-" + std::to_string(i)] = path + "/max_energy_range_uj";
      inner_id++;
      path = RAPL_API_PATH + temp + "/" + temp + ":" + std::to_string(inner_id);
    }
  }
}

std::string RAPLDevice::getName(std::string path)
{
  /**
   * Helper function to get the name of the domain
   * given the path of the domain
   */
  std::string name_path = path + "/name";
  std::ifstream file_handler(name_path);
  std::string name;
  getline(file_handler, name);
  return name;
}

std::map<std::string, unsigned long long> RAPLDevice::getEnergy()
{
  /**
   * Function to get the energy counter values from Powercap
   */
  std::map<std::string, unsigned long long> energies;

  for (auto device : devices)
  {
    std::string path = device.second;
    std::ifstream Filehandler(path);

    if (!Filehandler.is_open())
    {
      throw CPPJoulesException("RAPL access denied");
    }

    std::string energy_s;
    getline(Filehandler, energy_s);
    long long energy = std::stoll(energy_s);
    energies[device.first] = energy;
  }
  return energies;
}
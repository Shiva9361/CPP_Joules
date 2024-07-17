#include <cppJoules_exceptions.h>
#include <cppJoules.h>
#include <iostream>
#include <fstream>
#include <filesystem>

void EnergyTracker::start()
{
  if (state == STARTED)
  {
    throw CPPJoulesException("Tracker already started");
  }
  auto start_time = std::chrono::system_clock::now();
  auto start_energy = RAPL_device.getEnergy();
  std::map<std::string, unsigned long long> nvidia_energy;
  if (NVML_device.usable)
  {
    nvidia_energy = NVML_device.getEnergy();
    start_energy.insert(nvidia_energy.begin(), nvidia_energy.end());
  }

  EnergyState energy = EnergyState(start_time, start_energy);
  energy_readings.push_back(energy);
  state = STARTED;
}

void EnergyTracker::stop()
{
  if (state == UNINITIALIZED)
    throw CPPJoulesException("Tracker never started");
  if (state == STOPPED)
    throw CPPJoulesException("Tracker already stopped");
  auto end_time = std::chrono::system_clock::now();
  auto stop_energy = RAPL_device.getEnergy();
  std::map<std::string, unsigned long long> nvidia_energy;

  if (NVML_device.usable)
  {
    nvidia_energy = NVML_device.getEnergy();
    stop_energy.insert(nvidia_energy.begin(), nvidia_energy.end());
  }

  EnergyState energy = EnergyState(end_time, stop_energy);
  energy_readings.push_back(energy);
  state = STOPPED;
}

void EnergyTracker::calculate_energy()
{
  if (state == UNINITIALIZED)
  {
    throw CPPJoulesException("Tracker never started");
  }
  if (state != STOPPED)
  {
    throw CPPJoulesException("Stop tracker before calculating Energy");
  }
  last_calculated_energies.clear();
  last_calculated_time = 0;
  for (uint32_t i = 0; i < energy_readings.size() - 1; i++)
  {
    auto start = energy_readings[i];
    auto stop = energy_readings[i + 1];
    int64_t delta_time = std::chrono::duration_cast<std::chrono::seconds>(stop.timestamp - start.timestamp).count();

    if (start.energies.size() != stop.energies.size())
      throw CPPJoulesException("Readings are off");
    last_calculated_time += delta_time;
    for (auto domain : start.energies)
    {
      /**
       * If the energy counter had reset, we have to add the max energy
       */
      if (stop.energies[domain.first] - domain.second < 0)
      {
        if (RAPL_device.max_energy_devices.count(domain.first))
        {
          std::ifstream Filehandler(RAPL_device.max_energy_devices[domain.first]);
          std::string energy_s;
          getline(Filehandler, energy_s);
          long long energy = std::stoll(energy_s);

          last_calculated_energies[domain.first] += stop.energies[domain.first] - domain.second + energy;
        }
      }
      else
      {
        last_calculated_energies[domain.first] += (stop.energies[domain.first] - domain.second);
      }
    }
  }
  energy_readings.clear();
}
void EnergyTracker::print_energy(std::string tag)
{
  if (last_calculated_energies.empty())
  {
    throw("No Value to print");
  }
  std::cout << "Tag " << tag << "\n";
  std::cout << "Time " << last_calculated_time << "\n";
  for (auto energy : last_calculated_energies)
  {
    std::cout << energy.first << " " << energy.second << "\n";
  }
}

void EnergyTracker::save_csv(std::string file, std::string tag)
{
  std::ofstream csv_file(file, std::ios_base::app);

  csv_file << "tag" << ",";
  for (auto energy : last_calculated_energies)
  {
    csv_file << energy.first << ",";
  }
  csv_file << "\n";
  csv_file << tag << ",";
  for (auto energy : last_calculated_energies)
  {
    csv_file << energy.second << ",";
  }
  csv_file << "\n";
  csv_file.close();
}
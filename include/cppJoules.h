#ifndef CPP_JOULES
#define CPP_JOULES
#include <iostream>
#include <chrono>
#include <map>
#include "rapl_devices.h"
#include "energy_state.h"
#include "nvidia_devices.h"

#ifdef _WIN32
#define EXPOSE_DLL __declspec(dllexport)
#else
#define EXPOSE_DLL
#endif

enum TrackerState
{
  UNINITIALIZED,
  STARTED,
  STOPPED
};

class EXPOSE_DLL EnergyTracker
{
  // std::chrono::time_point<std::chrono::system_clock> start_time, end_time;
  RAPLDevice RAPL_device;
  NVMLDevice NVML_device;
  std::vector<EnergyState> energy_readings;
  TrackerState state = UNINITIALIZED;
  std::map<std::string, long long> last_calculated_energies;
  uint64_t last_calculated_time;

public:
  void start();
  void stop();
  void calculate_energy();
  void print_energy();
  void save_csv(std::string file);
};
#endif
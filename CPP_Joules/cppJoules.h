#ifndef CPP_JOULES
#define CPP_JOULES
#include <functional>
#include <iostream>
#include <chrono>
#include <map>
#include "rapl_devices.h"
#include "energy_state.h"
#include "nvidia_devices.h"

enum TrackerState
{
  UNINITIALIZED,
  STARTED,
  STOPPED
};

class EnergyTracker
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
};

template <typename Func>
auto measure_energy(Func func)
{
  return [func](auto &&...args)
  {
    EnergyTracker energy_tracker;
    energy_tracker.start();
    auto value = func(std::forward<decltype(args)>(args)...);
    energy_tracker.stop();
    energy_tracker.calculate_energy();
    energy_tracker.print_energy();
    return value;
  };
}
#endif
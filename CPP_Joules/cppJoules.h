#ifndef CPP_JOULES
#define CPP_JOULES
#include <functional>
#include <iostream>
#include <chrono>
#include "rapl_devices.h"
#include "energy_type.h"

class EnergyTracker
{
  std::chrono::time_point<std::chrono::system_clock> start_time, end_time;
  RAPLDevice RAPL_device;
  EnergyType inital_energy, final_energy;
  long long total_core_energy;

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
    // EnergyTracker energy_tracker;
    // energy_tracker.start();
    auto value = func(std::forward<decltype(args)>(args)...);
    // energy_tracker.stop();
    // energy_tracker.calculate_energy();
    // energy_tracker.print_energy();
    return value;
  };
}
#endif
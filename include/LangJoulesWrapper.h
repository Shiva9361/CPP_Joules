#ifndef LANG_JOULES_WRAPPER
#define LANG_JOULES_WRAPPER

#include "cppJoules.h"

extern "C"
{
  typedef struct EnergyTrackerC EnergyTrackerC;
  EnergyTrackerC *tracker();
  void tracker_delete(EnergyTrackerC *tracker);
  void start(EnergyTrackerC *tracker);
  void stop(EnergyTrackerC *tracker);
  void calculate_energy(EnergyTrackerC *tracker);
  void print_energy(EnergyTrackerC *tracker);
}
#endif
#include <LangJoulesWrapper.h>

struct EnergyTrackerC
{
  EnergyTracker tracker;
};

EnergyTrackerC *tracker()
{
  return new EnergyTrackerC();
}

void tracker_delete(EnergyTrackerC *trackerwrapper)
{
  delete trackerwrapper;
}

void start(EnergyTrackerC *trackerwrapper)
{
  trackerwrapper->tracker.start();
}
void stop(EnergyTrackerC *trackerwrapper)
{
  trackerwrapper->tracker.stop();
}
void calculate_energy(EnergyTrackerC *trackerwrapper)
{
  trackerwrapper->tracker.calculate_energy();
}
void print_energy(EnergyTrackerC *trackerwrapper)
{
  trackerwrapper->tracker.print_energy();
}
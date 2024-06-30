#include "../CPP_Joules/cppJoules.h"
#include <iostream>
#include <thread>
int add(int a, int b);

int main(int argc, char const *argv[])
{
  /* code */
  // auto new_func = measure_energy(add);
  //  std::cout << new_func(1, 2) << std::endl;
  EnergyTracker energy_tracker;
  energy_tracker.start();
  std::this_thread::sleep_for(std::chrono::seconds(2));
  energy_tracker.stop();
  energy_tracker.start();
  std::this_thread::sleep_for(std::chrono::seconds(2));
  energy_tracker.stop();
  energy_tracker.calculate_energy();
  energy_tracker.print_energy();
}

int add(int a, int b)
{
  std::this_thread::sleep_for(std::chrono::seconds(2));
  int c = 0;
  for (int i = 0; i < 1000000; i++)
    c += (a + b);
  return c;
}

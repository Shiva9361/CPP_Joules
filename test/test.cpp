#include "../CPP_Joules/cppJoules.h"
#include <iostream>
#include <thread>
int add(int a, int b);

int main(int argc, char const *argv[])
{
  /* code */
  auto new_func = measure_energy(add);
  std::cout << new_func(1, 2) << std::endl;
  return 0;
}

int add(int a, int b)
{
  std::this_thread::sleep_for(std::chrono::seconds(2));
  return a + b;
}

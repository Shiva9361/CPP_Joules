// Adding guard
#ifndef CPP_JOULES
#define CPP_JOULES
#include <functional>
#include <iostream>
template <typename Func>
auto measure_energy(Func func)
{
  return [func](auto &&...args)
  {
    std::cout << "Logging" << std::endl;
    return func(std::forward<decltype(args)>(args)...);
  };
}
#endif
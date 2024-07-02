#include <nvidia_devices.h>
#include <cppJoules_exceptions.h>
#include <string>
#include <iostream>
/**
 * Every function in the NVML API returns a STATUS CODE when run
 * We can procede only if the status code is NVML_SUCCESS
 */
#define check(status_code)                                                                      \
  if (static_cast<nvmlReturn_t>(status_code) != NVML_SUCCESS)                                   \
  {                                                                                             \
    throw CPPJoulesException(("Error occured in nvml, code : " + std::to_string(status_code))); \
  }
// NVML Requires pointers to be passed for all APIs

NVMLDevice::NVMLDevice()
{
  try
  {
    check(nvmlInit_v2());
    device_count = std::make_unique<uint32_t>();
    check(nvmlDeviceGetCount_v2(device_count.get()));

    devices.resize(*device_count.get());
    for (uint32_t i = 0; i < *device_count.get(); i++)
    {
      devices[i] = std::make_unique<nvmlDevice_t>();
      check(nvmlDeviceGetHandleByIndex_v2(i, devices[i].get()));
    }
  }
  catch (CPPJoulesException e)
  {
    std::cout << "Nvidia gpu not activated";
  }
}

std::map<std::string, unsigned long long> NVMLDevice::getEnergy()
{
  std::map<std::string, unsigned long long> energies;
  for (uint32_t i = 0; i < *device_count.get(); i++)
  {
    std::unique_ptr<unsigned long long> energy_value = std::make_unique<unsigned long long>();
    check(nvmlDeviceGetTotalEnergyConsumption(*devices[i].get(), energy_value.get()));
    energies["nvidia_gpu_" + std::to_string(i)] = *energy_value.get();
  }
  return energies;
}
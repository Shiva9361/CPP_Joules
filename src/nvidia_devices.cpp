#include <nvidia_devices.h>
#include <cppJoules_exceptions.h>
#include <string>

#include <iostream>
/**
 * Every function in the NVML API returns a STATUS CODE when run
 * We can procede only if the status code is NVML_SUCCESS
 */
/**
 * NVML Requires pointers to be passed for all APIs
 * Also dynamic loading nvml to make rapl usable even when
 * the nvml library is not available
 * */
typedef void (*nvmlfunction_void)();
typedef void (*nvmlfunction_uint32)(uint32_t *);
typedef void (*nvmlfunction_unit32_nvmlDevice)(uint32_t, nvmlDevice_t *);
typedef void (*nvmlfunction_nvmlDevice_unsignedlonglong)(nvmlDevice_t, unsigned long long *);

NVMLDevice::NVMLDevice()
{
  nvmlhandle.reset(dlopen("libnvidia-ml.so.1", RTLD_LAZY));
  usable = true;
  if (!nvmlhandle)
  {
    std::cout << "Nvidia gpu not activated" << std::endl;
    usable = false;
    return;
  }

  nvmlfunction_void _nvmlInit = reinterpret_cast<nvmlfunction_void>(dlsym(nvmlhandle.get(), "nvmlInit_v2"));
  nvmlfunction_uint32 _nvmlDeviceGetCount = reinterpret_cast<nvmlfunction_uint32>(dlsym(nvmlhandle.get(), "nvmlDeviceGetCount_v2"));
  nvmlfunction_unit32_nvmlDevice _nvmlDeviceGetHandleByIndex = reinterpret_cast<nvmlfunction_unit32_nvmlDevice>(dlsym(nvmlhandle.get(), "nvmlDeviceGetHandleByIndex_v2"));

  _nvmlInit();

  device_count = std::make_unique<uint32_t>();
  _nvmlDeviceGetCount(device_count.get());

  devices.resize(*device_count.get());
  for (uint32_t i = 0; i < *device_count.get(); i++)
  {
    devices[i] = std::make_unique<nvmlDevice_t>();
    _nvmlDeviceGetHandleByIndex(i, devices[i].get());
  }
}

std::map<std::string, unsigned long long> NVMLDevice::getEnergy()
{
  nvmlfunction_nvmlDevice_unsignedlonglong _nvmlDeviceGetTotalEnergyConsumption = reinterpret_cast<nvmlfunction_nvmlDevice_unsignedlonglong>(dlsym(nvmlhandle.get(), "nvmlDeviceGetTotalEnergyConsumption"));
  std::map<std::string, unsigned long long> energies;
  for (uint32_t i = 0; i < *device_count.get(); i++)
  {
    std::unique_ptr<unsigned long long> energy_value = std::make_unique<unsigned long long>();
    _nvmlDeviceGetTotalEnergyConsumption(*devices[i].get(), energy_value.get());
    energies["nvidia_gpu_" + std::to_string(i)] = *energy_value.get();
  }
  return energies;
}
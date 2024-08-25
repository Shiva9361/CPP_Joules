#include <rapl_devices.h>
#include <cppJoules_exceptions.h>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <memory>
#include <unordered_map>
#include <dlfcn.h>
#include <MacEnergyLib.h>

#ifdef _WIN64

#define MSR_FUNC_FREQ 0
#define MSR_FUNC_POWER 1
#define MSR_FUNC_TEMP 2
#define MSR_FUNC_LIMIT 3

#define GETFUNC(lib, fn) GetProcAddress((lib), (fn))
typedef void (*powergadgetfunction_void)();
typedef bool (*powergadgetfunction_bool)();
typedef void (*powergadgetfunction_intp)(int *);
typedef void (*powergadgetfunction_int_intp)(int, int *);
typedef void (*powergadgetfunction_int_wchartp)(int, wchar_t *);
typedef void (*powergadgetfunction_int_int_doublep_intp)(int, int, double *, int *);
#endif

RAPLDevice::RAPLDevice()
{
  /**
   * Initialization of RAPL using the powercap interface
   * All the possible domains are searched for and
   * the domains that are present are added for measurement
   * Requires read access to all energy_uj files of accessible
   * domains
   *  */
#ifdef __linux__
  int socket_id = 0;
  std::string path = RAPL_API_PATH + "intel-rapl:" + std::to_string(socket_id);

  while (std::filesystem::exists(path))
  {
    socket_id++;
    path = RAPL_API_PATH + "intel-rapl:" + std::to_string(socket_id);
  }

  for (uint32_t i = 0; i < socket_id; i++)
  {
    std::string temp = "intel-rapl:" + std::to_string(i);
    int inner_id = 0;
    path = RAPL_API_PATH + temp + "/" + temp + ":" + std::to_string(inner_id);

    std::unordered_map<std::string, std::string> device, max_energy_device;

    devices[getName(RAPL_API_PATH + temp)] = RAPL_API_PATH + temp + "/energy_uj";
    max_energy_devices[getName(RAPL_API_PATH + temp)] = RAPL_API_PATH + temp + "/max_energy_range_uj";

    while (std::filesystem::exists(path))
    {
      devices[getName(path) + "-" + std::to_string(i)] = path + "/energy_uj";
      max_energy_device[getName(path) + "-" + std::to_string(i)] = path + "/max_energy_range_uj";
      inner_id++;
      path = RAPL_API_PATH + temp + "/" + temp + ":" + std::to_string(inner_id);
    }
  }
#endif
/**
 * In windows we need to use the energy dll to get
 * the required functions
 */
#ifdef _WIN64
  initialized = false;
  const char *path = "..\\dependencies\\EnergyLib64.dll";
  handler = LoadLibrary(path);
  if (!handler)
  {
    std::cout << "Power gadget could not be loaded" << std::endl;
    return;
  }
  powergadgetfunction_void _gadgetInit = reinterpret_cast<powergadgetfunction_void>(GETFUNC(handler, "IntelEnergyLibInitialize"));
  // powergadgetfunction_void _ReadSample = reinterpret_cast<powergadgetfunction_void>(GETFUNC(handler, "ReadSample"));
  _gadgetInit();

  powergadgetfunction_intp _GetNumMsrs = reinterpret_cast<powergadgetfunction_intp>(GETFUNC(handler, "GetNumMsrs"));
  powergadgetfunction_intp _GetNumNodes = reinterpret_cast<powergadgetfunction_intp>(GETFUNC(handler, "GetNumNodes"));
  int numMsrs = 0;
  _GetNumMsrs(&numMsrs);

  int numNodes = 0;
  _GetNumNodes(&numNodes);

  powergadgetfunction_int_intp _GetMsrFunc = reinterpret_cast<powergadgetfunction_int_intp>(GETFUNC(handler, "GetMsrFunc"));
  powergadgetfunction_int_wchartp _GetMsrName = reinterpret_cast<powergadgetfunction_int_wchartp>(GETFUNC(handler, "GetMsrName"));
  powergadgetfunction_bool _ReadSample = reinterpret_cast<powergadgetfunction_bool>(GETFUNC(handler, "ReadSample"));

  //_ReadSample();
  for (int i = 0; i < numNodes; i++)
  {
    for (int j = 0; j < numMsrs; j++)
    {
      int func;
      _GetMsrFunc(j, &func);
      if (func == MSR_FUNC_POWER)
      {
        std::unique_ptr<wchar_t[]> name = std::make_unique<wchar_t[]>(100);
        _GetMsrName(j, name.get());
        std::wstring namewstr = std::wstring(name.get());
        std::string namestr = std::string(namewstr.begin(), namewstr.end());
        namestr = namestr + "-" + std::to_string(i);
        devices[namestr] = {i, j};
      }
    }
  }

#endif
/**
 * For Mac-intel we will be using intelPower Gadget framework
*/
#ifdef __APPLE__

#include <dlfcn.h>
#include <iostream>
#include <string>
#include <map>

// Ensure these types match the actual signatures in the dynamic library
typedef bool (*powergadgetfunction_void)();
typedef bool (*powergadgetfunction_intp)(int *);
typedef bool (*powergadgetfunction_int_charp)(int, char *);
typedef bool (*powergadgetfunction_bool)(void);

// Function signature for GetMsrFunc
typedef bool (*powergadgetfunction_int_intp)(int, int *);

bool initialized = false;
const char *path = "../dependencies/IntelPowerGadget.framework/IntelPowerGadget";
void *handler = dlopen(path, RTLD_LAZY);
if (!handler)
{
    std::cerr << "Power gadget could not be loaded" << std::endl;
    return;
}

// Function pointers
powergadgetfunction_void _gadgetInit = reinterpret_cast<powergadgetfunction_void>(dlsym(handler, "IntelEnergyLibInitialize"));
if (!_gadgetInit)
{
    std::cerr << "Failed to get function IntelEnergyLibInitialize" << std::endl;
    dlclose(handler);
    return;
}
_gadgetInit();

powergadgetfunction_intp _GetNumMsrs = reinterpret_cast<powergadgetfunction_intp>(dlsym(handler, "GetNumMsrs"));
powergadgetfunction_intp _GetNumNodes = reinterpret_cast<powergadgetfunction_intp>(dlsym(handler, "GetNumNodes"));
if (!_GetNumMsrs || !_GetNumNodes)
{
    std::cerr << "Failed to get functions GetNumMsrs or GetNumNodes" << std::endl;
    dlclose(handler);
    return;
}

int numMsrs = 0;
if (!_GetNumMsrs(&numMsrs))
{
    std::cerr << "Failed to get number of MSRs" << std::endl;
    dlclose(handler);
    return;
}

int numNodes = 0;
if (!_GetNumNodes(&numNodes))
{
    std::cerr << "Failed to get number of nodes" << std::endl;
    dlclose(handler);
    return;
}

powergadgetfunction_int_charp _GetMsrName = reinterpret_cast<powergadgetfunction_int_charp>(dlsym(handler, "GetMsrName"));
powergadgetfunction_int_intp _GetMsrFunc = reinterpret_cast<powergadgetfunction_int_intp>(dlsym(handler, "GetMsrFunc"));
powergadgetfunction_bool _ReadSample = reinterpret_cast<powergadgetfunction_bool>(dlsym(handler, "ReadSample"));
if (!_GetMsrName || !_GetMsrFunc || !_ReadSample)
{
    std::cerr << "Failed to get one or more functions" << std::endl;
    dlclose(handler);
    return;
}

// Assuming devices is a member variable or globally declared
std::map<std::string, std::pair<int, int>> devices;

for (int i = 0; i < numNodes; i++)
{
    for (int j = 0; j < numMsrs; j++)
    {
        int func;
        if (!_GetMsrFunc(j, &func))
        {
            std::cerr << "Failed to get MSR function" << std::endl;
            continue;
        }

        if (func == MSR_FUNC_POWER)
        {
            char name[100];
            if (!_GetMsrName(j, name))
            {
                std::cerr << "Failed to get MSR name" << std::endl;
                continue;
            }

            std::string namestr = std::string(name) + "-" + std::to_string(i);
            devices[namestr] = {i, j};
        }
    }
}

dlclose(handler);

#endif



}

std::string RAPLDevice::getName(std::string path)
{
  /**
   * Helper function to get the name of the domain
   * given the path of the domain
   */
  std::string name_path = path + "/name";
  std::ifstream file_handler(name_path);
  std::string name;
  getline(file_handler, name);
  return name;
}

std::map<std::string, unsigned long long> RAPLDevice::getEnergy()
{
  /**
   * Function to get the energy counter values from Powercap in linux
   * For windows, uses the functions exposed by the energy dll
   */
  std::map<std::string, unsigned long long> energies;
#ifdef __linux__
  for (auto device : devices)
  {
    std::string path = device.second;
    std::ifstream Filehandler(path);

    if (!Filehandler.is_open())
    {
      return energies;
    }

    std::string energy_s;
    getline(Filehandler, energy_s);
    long long energy = std::stoll(energy_s);
    energies[device.first] = energy;
  }
#elif _WIN64
  if (!handler)
  {
    return energies;
  }
  powergadgetfunction_int_int_doublep_intp _GetPowerData = reinterpret_cast<powergadgetfunction_int_int_doublep_intp>(GETFUNC(handler, "GetPowerData"));
  powergadgetfunction_bool _ReadSample = reinterpret_cast<powergadgetfunction_bool>(GETFUNC(handler, "ReadSample"));
  _ReadSample();

  for (auto device : devices)
  {
    std::unique_ptr<double[]> data = std::make_unique<double[]>(10);
    int nData;
    if (initialized)
    {
      _GetPowerData(device.second.first, device.second.second, data.get(), &nData);
      energies[device.first] = static_cast<long long>(data.get()[1]);
    }
    else
    {
      energies[device.first] = 0;
    }
  }
  if (!initialized)
  {
    initialized = true;
  }
#endif

  return energies;
}
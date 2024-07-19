# CPP Joules

### Energy Measurement tool for CPP/CUDA programs

- Uses Intel RAPL interface to get energy values
- Uses NVML interface for getting GPU energy values
- Needs read access to powercap in linux
- Needs a 64bit windows system with nvml in system32 

## Instructions
### Linux
```bash!=
  mkdir build
  cd build
  cmake ..
  sudo cmake --build . --target install
```
### Windows
- Dll can be generated using the make file
- Dll needs to be in the same directory as the .exe file
- Headers in the include folder can be added to any other project
  
### CPPJoules library will be installed in your system

#### Linux
- To use with g++, nvcc use -lCPP_Joules flag
#### Windows
- To use with g++, use -lCPP_Joules -LPath/to/dll


### To use
```cpp!=
  #include<CPP_Joules/cppJoules.h>
```


# CPP Joules

### Energy Measurement tool for CPP/CUDA programs

- Uses Intel RAPL interface to get energy values
- Uses NVML interface for getting GPU energy values
- Needs read access to powercap

## Instructions

```bash=
  mkdir build
  cd build
  cmake ..
  sudo cmake --build . --target install
```

CPPJoules library will be installed in your system

- To use with g++, nvcc use -l:libCPP_Joules.a flag

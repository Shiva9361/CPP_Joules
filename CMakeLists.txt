cmake_minimum_required(VERSION 3.2)
project(CPP_Joules VERSION 0.0.1 DESCRIPTION "Energy Measurement toolkit for cpp" LANGUAGES CXX)

add_library(CPP_Joules STATIC ${PROJECT_SOURCE_DIR}/CPP_Joules/cppJoules.cpp 
${PROJECT_SOURCE_DIR}/CPP_Joules/energy_state.cpp 
${PROJECT_SOURCE_DIR}/CPP_Joules/energy_type.cpp 
${PROJECT_SOURCE_DIR}/CPP_Joules/rapl_devices.cpp)
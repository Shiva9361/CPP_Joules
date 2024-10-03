git clone https://github.com/Shiva9361/CPP_Joules.git
mkdir include
cd CPP_Joules
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
make
copy ..\include\* ..\..\include
copy libCPP_Joules.dll ..\..\

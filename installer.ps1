git clone https://github.com/Shiva9361/CPP_Joules.git
mkdir include
Set-Location CPP_Joules
mkdir build
Set-Location build
cmake -G "MinGW Makefiles" ..
make
Copy-Item ..\include\* ..\..\include
Copy-Item libCPP_Joules.dll ..\..\

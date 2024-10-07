curl -l0 https://github.com/Shiva9361/CPP_Joules/releases/download/v1.0/CPP_Joules_windows.tar.gz --output CPP_Joules_windows.tar.gz
tar -xvzf CPP_Joules_windows.tar.gz 
del CPP_Joules_windows.tar.gz
@REM cd cppJoules
@REM copy /s /h "libCPP_Joules.dll" "%windir%\system32\"
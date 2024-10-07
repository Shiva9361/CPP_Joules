@echo off
curl -L0 https://github.com/Shiva9361/CPP_Joules/releases/download/v1.0/CPP_Joules_windows.tar.gz --output %~dp0/CPP_Joules_windows.tar.gz
tar -xvzf %~dp0/CPP_Joules_windows.tar.gz 
del "%~dp0CPP_Joules_windows.tar.gz"
cd "%~dp0cppJoules"
xcopy /h "%~dp0\cppJoules\libCPP_Joules.dll" "%windir%\system32\"
xcopy /h "%~dp0\cppJoules\EnergyLib64.dll" "%windir%\system32\"
pause
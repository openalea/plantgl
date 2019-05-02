:: Working Dir
mkdir build
cd build

:: Build
:: cmake -G "NMake Makefiles" -DCMAKE_INSTALL_PREFIX=%LIBRARY_PREFIX% -DCMAKE_BUILD_TYPE=Release ..
cmake .. -G "Visual Studio 14 2015 Win64" -T host=x64 -DCMAKE_INSTALL_PREFIX=%LIBRARY_PREFIX%
if errorlevel 1 exit 1
:: nmake
cmake --build . --target install --config Release
if errorlevel 1 exit 1
:: nmake install
::if errorlevel 1 exit 1

:: Install Python Files
cd ..
%PYTHON% setup.py install
if errorlevel 1 exit 1

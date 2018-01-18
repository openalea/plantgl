COPY options_conda_win.py options.py

call "%VS90COMNTOOLS%vcvarsall.bat" %PROCESSOR_ARCHITECTURE%

echo %PROCESSOR_ARCHITECTURE%

echo %WindowsSdkDir%

systeminfo

%PYTHON% setup.py install

if errorlevel 1 exit 1


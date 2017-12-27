COPY options_conda_win.py options.py

echo %PROCESSOR_ARCHITECTURE%

systeminfo

%PYTHON% setup.py install

if errorlevel 1 exit 1


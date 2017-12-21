COPY options_conda_win.py options.py

scons -h

%PYTHON% setup.py install

if errorlevel 1 exit 1


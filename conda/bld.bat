COPY options_conda_win.py options.py

scons -H

if errorlevel 1 exit 1

python setup.py install

if errorlevel 1 exit 1


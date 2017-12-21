COPY options_conda_win.py options.py

scons -h

REM if errorlevel 1 exit 1

python setup.py install

if errorlevel 1 exit 1


COPY options_examples\options_conda_win.py options.py

REM systeminfo

%PYTHON% setup.py install

if errorlevel 1 exit 1


COPY options_conda_win.py options.py

if %PROCESSOR_ARCHITECTURE% == "x86" (
	call %VS90COMNTOOLS%\vcvarsall.bat x86_amd64
)

echo %PROCESSOR_ARCHITECTURE%

systeminfo

%PYTHON% setup.py install

if errorlevel 1 exit 1


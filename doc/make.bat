@ECHO OFF

REM Command file for Sphinx documentation (Windows equivalent of the Makefile)

pushd %~dp0

if "%SPHINXBUILD%" == "" (
	set SPHINXBUILD=sphinx-build
)
set SOURCEDIR=.
set BUILDDIR=_build

if "%1" == "" goto help

%SPHINXBUILD% >NUL 2>NUL
if errorlevel 9009 (
	echo.
	echo.The 'sphinx-build' command was not found. Activate the conda environment
	echo.including doc/doc_deps.yml, see README.md.
	exit /b 1
)

if "%1" == "clean" goto clean
if "%1" == "rtdhtml" goto rtdhtml

REM Any other target (html, dirhtml, singlehtml, linkcheck...) goes to sphinx-build
%SPHINXBUILD% -M %1 %SOURCEDIR% %BUILDDIR% %SPHINXOPTS% %O%
goto end

:rtdhtml
REM Same settings as Read the Docs: warnings become errors
%SPHINXBUILD% -T -j auto -E -W --keep-going -b html -d %BUILDDIR%\doctrees -D language=en %SOURCEDIR% %BUILDDIR%\html
goto end

:clean
if exist %BUILDDIR% rmdir /s /q %BUILDDIR%
if exist generated rmdir /s /q generated
if exist auto_gallery rmdir /s /q auto_gallery
goto end

:help
%SPHINXBUILD% -M help %SOURCEDIR% %BUILDDIR% %SPHINXOPTS% %O%
echo.
echo.  rtdhtml     to make HTML files with the settings of Read the Docs

:end
popd

@echo off
cls

:: env init
call tools\core_launch.bat

setlocal

:: get current scrip path
set "SDK_TOP_DIR=%~dp0"

:: build output dir 
set OUTPUT=out

:: del default output dir
if exist "%OUTPUT%" (
    RD /S /Q "%OUTPUT%"
)

:: mkdir for build output
if not exist "%OUTPUT%" (
    mkdir "%OUTPUT%"
)

::define some variables
set DIR_RELEASE=release

:: cd to output dir
cd %OUTPUT%
echo Current directory: %cd%

:: create release package dir
if not exist %DIR_RELEASE% md %DIR_RELEASE%

:: cmake init and start build
cmake .. -G Ninja -U BUILD_CUST_PROJ
ninja

::check file exist in release
if not exist bin\nwy_open_app.bin goto ERROR

::compress nwy open app to nwy_open_app_tmp.bin
copy /Y /B bin\nwy_open_app.bin input_file.bin
lzma_asr.exe ds 24 e input_file.bin nwy_open_app_tmp.bin

setlocal ENABLEDELAYEDEXPANSION
:: get file lenght and save to cfile_header
for /f "delims=" %%i in ('powershell -NoProfile -Command "(Get-Item 'nwy_open_app_tmp.bin').Length"') do (set cfile_len=%%i)
<nul set /p ="CSTA!cfile_len!CEND">cfile_header
setlocal DISABLEDELAYEDEXPANSION

copy /B /Y cfile_header+nwy_open_app_tmp.bin nwy_open_app.bin
::del cfile_header
::del nwy_open_app_tmp.bin
::del input_file.bin

:: generate fota package
copy /Y /B  %SDK_TOP_DIR%\tools\target-upgrade.xml target-upgrade.xml
nwy_mkpackage.exe
::del target-upgrade.xml
copy /Y nwy_open_app_fota.pkt %DIR_RELEASE%\
::del nwy_open_app_fota.pkt

:: Generate release package
copy /Y /B nwy_open_app.bin %SDK_TOP_DIR%\tools\aboot\app.bin
cd  %SDK_TOP_DIR%\tools\aboot
call MakePkgBin
copy /Y nwy_open_app.zip %SDK_TOP_DIR%\%OUTPUT%\%DIR_RELEASE%\
del nwy_open_app.zip
goto SUCCESS

:ERROR
echo ****************************************************************
echo ***********************  ERROR  ********************************
echo ****************************************************************
cd /d "%SDK_TOP_DIR%"
goto END

:SUCCESS
::@make -f build\nwy_open_app_gnu.mak clean > nul
echo ****************************************************************
echo ***********************  SUCCESS  ******************************
echo ****************************************************************
cd /d "%SDK_TOP_DIR%"
goto END

:END
if not "%CD%"=="%SDK_TOP_DIR%" cd /d "%SDK_TOP_DIR%"
endlocal


@echo off
REM Check if running with admin privileges
NET SESSION >nul 2>&1
IF %ERRORLEVEL% EQU 0 (
    echo Running with admin privileges...
) ELSE (
    echo Please run this script as Administrator.
    pause
    exit
)

REM Change directory to your desired location
cd /d "file location"

REM Compile the source files using gcc
gcc main-server.cpp & second.cpp -o server.exe -lws2_32 (change to the location of your files)

REM Compile the source files using g++
g++ main-server.cpp second.cpp -o main-server.exe -lws2_32

REM Run the compiled server executable
.\server.exe

pause

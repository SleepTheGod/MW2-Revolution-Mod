@echo off
title Building SleepTheGod's MW2 Revolution Mod
color 0A
echo ============================================================
echo   SLEEPTHEGOD'S MW2 REVOLUTION MOD - BUILD SCRIPT
echo   Version 3.0
echo ============================================================
echo.

where cl >nul 2>nul
if %errorlevel% neq 0 (
    color 0C
    echo [!] ERROR: Visual Studio compiler not found!
    echo.
    echo Please run this from the Developer Command Prompt for Visual Studio.
    echo.
    pause
    exit /b 1
)

echo [1/4] Cleaning previous builds...
if exist "release" rmdir /s /q release
if exist "*.obj" del /q *.obj
if exist "*.dll" del /q *.dll
if exist "*.exp" del /q *.exp
if exist "*.lib" del /q *.lib

echo [2/4] Compiling source code...
cl /nologo /MD /O2 /GS- /EHsc /D "_CRT_SECURE_NO_WARNINGS" /c Revolution.cpp
if %errorlevel% neq 0 (
    color 0C
    echo [!] Compilation failed!
    pause
    exit /b 1
)

echo [3/4] Linking dynamic library...
link /nologo /DLL /OUT:Revolution.dll Revolution.obj ^
     kernel32.lib user32.lib gdi32.lib ws2_32.lib ^
     d3d9.lib d3dx9.lib /NODEFAULTLIB:libcmt.lib
if %errorlevel% neq 0 (
    color 0C
    echo [!] Linking failed!
    pause
    exit /b 1
)

echo [4/4] Creating release package...
if not exist "release" mkdir release
copy Revolution.dll release\
if exist "Revolution.pdb" copy Revolution.pdb release\
if exist "Revolution.exp" copy Revolution.exp release\
if exist "Revolution.lib" copy Revolution.lib release\

color 0A
echo.
echo ============================================================
echo   BUILD COMPLETE!
echo ============================================================
echo.
echo   Output: Revolution.dll
echo   Location: release\Revolution.dll
echo.
echo   Features included:
echo   - 50+ Menu Options
echo   - Rainbow Scrolling Text
echo   - F8 Menu System
echo   - F1-F7 Quick Features
echo   - Multiplayer Support
echo   - Scripting Engine
echo   - Network Features
echo   - Custom Game Modes
echo.
echo ============================================================
echo.
echo   To use:
echo   1. Inject Revolution.dll into MW2
echo   2. Press F8 to open the menu
echo   3. Enjoy!
echo.
echo ============================================================
echo.
pause

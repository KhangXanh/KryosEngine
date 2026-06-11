@echo off

cd /d "%~dp0"
echo [Kryos Engine] Setting up development environment...

:: Check CMake
where cmake >nul 2>nul
if %errorlevel% neq 0 (
    echo [ERROR] CMake is not installed or not in PATH!
    pause
    exit /b 1
)

:: Check Git
where git >nul 2>nul
if %errorlevel% neq 0 (
    echo [ERROR] Git is not installed or not in PATH!
    pause
    exit /b 1
)

:: Check Ninja
where ninja >nul 2>nul
if %errorlevel% neq 0 (
    echo [WARNING] Ninja not found, falling back to default generator...
    set GENERATOR=
) else (
    set GENERATOR=-G "Ninja"
)

echo [INFO] Updating submodules...
git submodule update --init --recursive
if %errorlevel% neq 0 (
    echo [ERROR] Failed to update submodules!
    pause
    exit /b 1
)

echo [INFO] Generating build files...
cmake -S Source -B build %GENERATOR%
if %errorlevel% neq 0 (
    echo [ERROR] CMake configuration failed!
    pause
    exit /b 1
)

echo [SUCCESS] Setup complete!
echo Run 'Build.bat' to compile the engine.
pause
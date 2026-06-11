@echo off

cd /d "%~dp0"
echo [Kryos Engine] Building...

cmake --build Build
if %errorlevel% neq 0 (
    echo [ERROR] Build failed!
    pause
    exit /b 1
)

echo [SUCCESS] Build complete! You can go to 'build\KryosStudio.exe' to use Kryos Engine.
pause
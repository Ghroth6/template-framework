@echo off
REM TFW Simple Example Project Build Script (Execute from root directory)

echo === Building TFW Simple Examples ===

REM Ensure we are in the build directory under root
if not exist "build" (
    echo Creating build directory...
    mkdir build
)

cd build

REM Create output directories
if not exist "out\c" mkdir "out\c"
if not exist "out\cpp" mkdir "out\cpp"
if not exist "out\lib" mkdir "out\lib"

REM Build core libraries first
echo Building core libraries...
cmake -B core_build -S ..\core
if %ERRORLEVEL% EQU 0 (
    cmake --build core_build --config Release
    if %ERRORLEVEL% EQU 0 (
        echo Core libraries built successfully
    ) else (
        echo Failed to build core libraries
        exit /b 1
    )
) else (
    echo Failed to configure core libraries
    exit /b 1
)

REM Build utils library
echo Building utils library...
cmake -B utils_build -S ..\utils
if %ERRORLEVEL% EQU 0 (
    cmake --build utils_build --config Release
    if %ERRORLEVEL% EQU 0 (
        echo Utils library built successfully
    ) else (
        echo Failed to build utils library
        exit /b 1
    )
) else (
    echo Failed to configure utils library
    exit /b 1
)

REM Build C version
echo Building C version...
cmake -B simple_c -S ..\simple\c
if %ERRORLEVEL% EQU 0 (
    cmake --build simple_c --config Release
    if %ERRORLEVEL% EQU 0 (
        echo C version built successfully
    ) else (
        echo Failed to build C version
        exit /b 1
    )
) else (
    echo Failed to configure C version
    exit /b 1
)

REM Build C++ version
echo Building C++ version...
cmake -B simple_cpp -S ..\simple\cpp
if %ERRORLEVEL% EQU 0 (
    cmake --build simple_cpp --config Release
    if %ERRORLEVEL% EQU 0 (
        echo C++ version built successfully
    ) else (
        echo Failed to build C++ version
        exit /b 1
    )
) else (
    echo Failed to configure C++ version
    exit /b 1
)

cd ..

REM Create shortcuts in root directory
echo Creating shortcuts in root directory...

REM Remove old shortcuts if they exist
if exist "tfw_simple_c.exe.lnk" del "tfw_simple_c.exe.lnk"
if exist "tfw_simple_cpp.exe.lnk" del "tfw_simple_cpp.exe.lnk"

REM Use PowerShell to create shortcuts with absolute paths
powershell -Command "$WshShell = New-Object -comObject WScript.Shell; $Shortcut = $WshShell.CreateShortcut('%CD%\tfw_simple_c.exe.lnk'); $Shortcut.TargetPath = '%CD%\out\c\Release\tfw_simple_c.exe'; $Shortcut.Save()"
powershell -Command "$WshShell = New-Object -comObject WScript.Shell; $Shortcut = $WshShell.CreateShortcut('%CD%\tfw_simple_cpp.exe.lnk'); $Shortcut.TargetPath = '%CD%\out\cpp\Release\tfw_simple_cpp.exe'; $Shortcut.Save()"

echo === All examples built successfully ===
echo Output files:
echo   C version: out\c\tfw_simple_c.exe
echo   C++ version: out\cpp\tfw_simple_cpp.exe
echo Shortcuts created in root directory:
echo   tfw_simple_c.exe.lnk
echo   tfw_simple_cpp.exe.lnk

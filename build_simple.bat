@echo off
REM TFW Simple 示例项目构建脚本 (从根目录执行)

echo === Building TFW Simple Examples ===

REM 确保在根目录的build目录中
if not exist "build" (
    echo Creating build directory...
    mkdir build
)

cd build

REM 创建输出目录
if not exist "out\c" mkdir "out\c"
if not exist "out\cpp" mkdir "out\cpp"

REM 构建C版本
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

REM 构建C++版本
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

REM 创建快捷方式到根目录
echo Creating shortcuts in root directory...

REM 删除旧的快捷方式（如果存在）
if exist "tfw_simple_c.exe.lnk" del "tfw_simple_c.exe.lnk"
if exist "tfw_simple_cpp.exe.lnk" del "tfw_simple_cpp.exe.lnk"

REM 使用PowerShell创建快捷方式
powershell -Command "$WshShell = New-Object -comObject WScript.Shell; $Shortcut = $WshShell.CreateShortcut('tfw_simple_c.exe.lnk'); $Shortcut.TargetPath = 'build\out\c\tfw_simple_c.exe'; $Shortcut.Save()"
powershell -Command "$WshShell = New-Object -comObject WScript.Shell; $Shortcut = $WshShell.CreateShortcut('tfw_simple_cpp.exe.lnk'); $Shortcut.TargetPath = 'build\out\cpp\tfw_simple_cpp.exe'; $Shortcut.Save()"

echo === All examples built successfully ===
echo Output files:
echo   C version: build\out\c\tfw_simple_c.exe
echo   C++ version: build\out\cpp\tfw_simple_cpp.exe
echo Shortcuts created in root directory:
echo   tfw_simple_c.exe.lnk
echo   tfw_simple_cpp.exe.lnk

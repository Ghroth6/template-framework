@echo off
REM TFW Simple 绀轰緥椤圭洰鏋勫缓鑴氭湰 (浠庢牴鐩綍鎵ц)

echo === Building TFW Simple Examples ===

REM 纭繚鍦ㄦ牴鐩綍鐨刡uild鐩綍涓?
if not exist "build" (
    echo Creating build directory...
    mkdir build
)

cd build

REM 鍒涘缓杈撳嚭鐩綍
if not exist "out\c" mkdir "out\c"
if not exist "out\cpp" mkdir "out\cpp"

REM 鏋勫缓C鐗堟湰
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

REM 鏋勫缓C++鐗堟湰
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

REM 鍒涘缓蹇嵎鏂瑰紡鍒版牴鐩綍
echo Creating shortcuts in root directory...

REM 鍒犻櫎鏃х殑蹇嵎鏂瑰紡锛堝鏋滃瓨鍦級
if exist "tfw_simple_c.exe.lnk" del "tfw_simple_c.exe.lnk"
if exist "tfw_simple_cpp.exe.lnk" del "tfw_simple_cpp.exe.lnk"

REM 浣跨敤PowerShell鍒涘缓蹇嵎鏂瑰紡
powershell -Command "$WshShell = New-Object -comObject WScript.Shell; $Shortcut = $WshShell.CreateShortcut('tfw_simple_c.exe.lnk'); $Shortcut.TargetPath = 'build\out\c\tfw_simple_c.exe'; $Shortcut.Save()"
powershell -Command "$WshShell = New-Object -comObject WScript.Shell; $Shortcut = $WshShell.CreateShortcut('tfw_simple_cpp.exe.lnk'); $Shortcut.TargetPath = 'build\out\cpp\tfw_simple_cpp.exe'; $Shortcut.Save()"

echo === All examples built successfully ===
echo Output files:
echo   C version: build\out\c\tfw_simple_c.exe
echo   C++ version: build\out\cpp\tfw_simple_cpp.exe
echo Shortcuts created in root directory:
echo   tfw_simple_c.exe.lnk
echo   tfw_simple_cpp.exe.lnk

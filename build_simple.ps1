# TFW项目构建脚本 (Windows PowerShell)

# 基于编码规范9.4节要求

# 设置错误处理

$ErrorActionPreference = "Stop"

# 颜色函数

function Write-ColorOutput {
    param(
        [string]$Message,
        [string]$Color = "White"
    )
    Write-Host $Message -ForegroundColor $Color
}

function Write-Info {
    param([string]$Message)
    Write-ColorOutput "[INFO] $Message" "Cyan"
}

function Write-Success {
    param([string]$Message)
    Write-ColorOutput "[SUCCESS] $Message" "Green"
}

function Write-Warning {
    param([string]$Message)
    Write-ColorOutput "[WARNING] $Message" "Yellow"
}

function Write-Error {
    param([string]$Message)
    Write-ColorOutput "[ERROR] $Message" "Red"
}

# 检查必要的工具

function Test-Requirements {
    Write-Info "Checking build tools..."

    # 检查CMake

    try {
        $cmakeVersion = cmake --version 2>$null
        if ($LASTEXITCODE -ne 0) {
            throw "CMake not found"
        }
        Write-Info "CMake version: $($cmakeVersion[0])"
    }
    catch {
        Write-Error "CMake not installed or not in PATH, please install CMake first"
        exit 1
    }

    # 检查生成器

    $generators = @("Visual Studio 17 2022", "Visual Studio 16 2019", "Ninja", "Unix Makefiles")
    $availableGenerator = $null

    foreach ($generator in $generators) {
        try {
            cmake --help | Select-String $generator | Out-Null
            if ($LASTEXITCODE -eq 0) {
                $availableGenerator = $generator
                break
            }
        }
        catch {
            continue
        }
    }

    if ($availableGenerator) {
        Write-Info "Using generator: $availableGenerator"
        $script:cmakeGenerator = $availableGenerator
    } else {
        Write-Warning "No suitable generator found, will use default generator"
        $script:cmakeGenerator = "Visual Studio 17 2022"
    }

    Write-Success "Build tools check completed"
}

# 运行清理脚本
function Run-CleanScript {
    Print-Info "运行清理脚本..."

    if (Test-Path ".\clean.ps1") {
        Print-Info "执行现有的clean.ps1脚本"
        powershell -ExecutionPolicy Bypass -File ".\clean.ps1"
        Print-Success "清理脚本执行完成"
    } elseif (Test-Path ".\clean.sh") {
        Print-Info "执行现有的clean.sh脚本"
        bash ".\clean.sh"
        Print-Success "清理脚本执行完成"
    } else {
        Print-Warning "未找到清理脚本，跳过清理步骤"
    }
}

# 创建必要的目录

function New-BuildDirectories {
    Write-Info "Creating build directories..."

    $directories = @("build", "out\bin", "out\lib")

    foreach ($dir in $directories) {
        if (!(Test-Path $dir)) {
            New-Item -ItemType Directory -Path $dir -Force | Out-Null
            Write-Info "Created directory: $dir"
        }
    }

    Write-Success "Directories creation completed"
}

# 配置CMake项目

function Invoke-CMakeConfigure {
    Write-Info "Configuring CMake project..."

    Push-Location build

    try {
        # 根据生成器选择配置参数

        $cmakeArgs = @(
            "..",
            "-DCMAKE_BUILD_TYPE=Release",
            "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"
        )

        if ($cmakeGenerator -like "Visual Studio*") {
            $cmakeArgs += "-A", "x64"
        }

        Write-Info "Executing: cmake $($cmakeArgs -join ' ')"
        cmake @cmakeArgs

        if ($LASTEXITCODE -ne 0) {
            throw "CMake configuration failed"
        }
    }
    finally {
        Pop-Location
    }

    Write-Success "CMake configuration completed"
}

# 编译项目

function Invoke-CMakeBuild {
    Write-Info "Starting project compilation..."

    Push-Location build

    try {
        if ($cmakeGenerator -like "Visual Studio*") {
            # Visual Studio生成器

            Write-Info "Using Visual Studio generator for compilation..."
            cmake --build . --config Release --parallel
        } else {
            # 其他生成器

            Write-Info "Using generator for compilation: $cmakeGenerator"
            cmake --build . --config Release
        }

        if ($LASTEXITCODE -ne 0) {
            throw "Compilation failed"
        }
    }
    finally {
        Pop-Location
    }

    Write-Success "Compilation completed"
}

# 显示构建结果

function Show-BuildResults {
    Write-Info "Build results:"

    # 检查编译数据库

    if (Test-Path "build\compile_commands.json") {
        Write-Success "✓ Compilation database generated (build\compile_commands.json)"
        Write-Info "  IDE can automatically load this file for intelligent suggestions"
    } else {
        Write-Warning "⚠ Compilation database not generated"
    }

    # 检查可执行文件

    if (Test-Path "out\bin") {
        $binFiles = Get-ChildItem "out\bin" -ErrorAction SilentlyContinue
        if ($binFiles) {
            Write-Success "✓ Executable files generated (out\bin\)"
            Get-ChildItem "out\bin" | Format-Table Name, Length, LastWriteTime
        } else {
            Write-Warning "⚠ Executable files not generated"
        }
    }

    # 检查库文件

    if (Test-Path "out\lib") {
        $libFiles = Get-ChildItem "out\lib" -ErrorAction SilentlyContinue
        if ($libFiles) {
            Write-Success "✓ Library files generated (out\lib\)"
            Get-ChildItem "out\lib" | Format-Table Name, Length, LastWriteTime
        } else {
            Write-Warning "⚠ Library files not generated"
        }
    }
}

# 主函数

function Main {
    Write-Info "Starting TFW project build..."
    Write-Info "Build type: Debug"
    Write-Info "Output directory: out\"

    Test-Requirements
    Run-CleanScript
    New-BuildDirectories
    Invoke-CMakeConfigure
    Invoke-CMakeBuild
    Show-BuildResults

    Write-Success "TFW project build completed!"
    Write-Info "Executable files location: out\bin\"
    Write-Info "Library files location: out\lib\"
}

# 脚本入口

if ($MyInvocation.InvocationName -ne '.') {
    Main
}

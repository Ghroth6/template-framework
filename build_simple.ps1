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
    Write-Info "检查构建工具..."

    # 检查CMake
    try {
        $cmakeVersion = cmake --version 2>$null
        if ($LASTEXITCODE -ne 0) {
            throw "CMake未找到"
        }
        Write-Info "CMake版本: $($cmakeVersion[0])"
    }
    catch {
        Write-Error "CMake未安装或不在PATH中，请先安装CMake"
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
        Write-Info "使用生成器: $availableGenerator"
        $script:cmakeGenerator = $availableGenerator
    } else {
        Write-Warning "未找到合适的生成器，将使用默认生成器"
        $script:cmakeGenerator = "Visual Studio 17 2022"
    }

    Write-Success "构建工具检查完成"
}

# 创建必要的目录
function New-BuildDirectories {
    Write-Info "创建构建目录..."

    $directories = @("build", "out\bin", "out\lib")

    foreach ($dir in $directories) {
        if (!(Test-Path $dir)) {
            New-Item -ItemType Directory -Path $dir -Force | Out-Null
            Write-Info "创建目录: $dir"
        }
    }

    Write-Success "目录创建完成"
}

# 配置CMake项目
function Invoke-CMakeConfigure {
    Write-Info "配置CMake项目..."

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

        Write-Info "执行: cmake $($cmakeArgs -join ' ')"
        cmake @cmakeArgs

        if ($LASTEXITCODE -ne 0) {
            throw "CMake配置失败"
        }
    }
    finally {
        Pop-Location
    }

    Write-Success "CMake配置完成"
}

# 编译项目
function Invoke-CMakeBuild {
    Write-Info "开始编译项目..."

    Push-Location build

    try {
        if ($cmakeGenerator -like "Visual Studio*") {
            # Visual Studio生成器
            Write-Info "使用Visual Studio生成器编译..."
            cmake --build . --config Release --parallel
        } else {
            # 其他生成器
            Write-Info "使用生成器编译: $cmakeGenerator"
            cmake --build . --config Release
        }

        if ($LASTEXITCODE -ne 0) {
            throw "编译失败"
        }
    }
    finally {
        Pop-Location
    }

    Write-Success "编译完成"
}

# 显示构建结果
function Show-BuildResults {
    Write-Info "构建结果："

    # 检查编译数据库
    if (Test-Path "build\compile_commands.json") {
        Write-Success "✓ 编译数据库已生成 (build\compile_commands.json)"
        Write-Info "  IDE可以自动加载此文件进行智能提示"
    } else {
        Write-Warning "⚠ 编译数据库未生成"
    }

    # 检查可执行文件
    if (Test-Path "out\bin") {
        $binFiles = Get-ChildItem "out\bin" -ErrorAction SilentlyContinue
        if ($binFiles) {
            Write-Success "✓ 可执行文件已生成 (out\bin\)"
            Get-ChildItem "out\bin" | Format-Table Name, Length, LastWriteTime
        } else {
            Write-Warning "⚠ 可执行文件未生成"
        }
    }

    # 检查库文件
    if (Test-Path "out\lib") {
        $libFiles = Get-ChildItem "out\lib" -ErrorAction SilentlyContinue
        if ($libFiles) {
            Write-Success "✓ 库文件已生成 (out\lib\)"
            Get-ChildItem "out\lib" | Format-Table Name, Length, LastWriteTime
        } else {
            Write-Warning "⚠ 库文件未生成"
        }
    }
}

# 主函数
function Main {
    Write-Info "开始构建TFW项目..."
    Write-Info "构建类型: Release"
    Write-Info "输出目录: out\"

    Test-Requirements
    New-BuildDirectories
    Invoke-CMakeConfigure
    Invoke-CMakeBuild
    Show-BuildResults

    Write-Success "TFW项目构建完成！"
    Write-Info "可执行文件位置: out\bin\"
    Write-Info "库文件位置: out\lib\"
}

# 脚本入口
if ($MyInvocation.InvocationName -ne '.') {
    Main
}

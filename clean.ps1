# TFW项目清理脚本 (Windows PowerShell)
# PowerShell 5.1兼容版本

# 设置错误处理
$ErrorActionPreference = "Stop"

# 设置编码为系统默认编码，确保中文注释不报错

[Console]::OutputEncoding = [System.Text.Encoding]::Default
$OutputEncoding = [System.Text.Encoding]::Default

# 颜色函数 - 用于输出彩色信息

function Write-ColorOutput {
    param(
        [string]$Message,
        [string]$Color = "White"
    )
    Write-Host $Message -ForegroundColor $Color
}

# 信息输出函数 - 显示一般信息

function Write-Info {
    param([string]$Message)
    Write-ColorOutput "[INFO] $Message" "Cyan"
}

# 成功输出函数 - 显示成功信息

function Write-Success {
    param([string]$Message)
    Write-ColorOutput "[SUCCESS] $Message" "Green"
}

# 警告输出函数 - 显示警告信息

function Write-Warning {
    param([string]$Message)
    Write-ColorOutput "[WARNING] $Message" "Yellow"
}

# 错误输出函数 - 显示错误信息

function Write-Error {
    param([string]$Message)
    Write-ColorOutput "[ERROR] $Message" "Red"
}

# 清理构建文件函数 - 删除build目录及其所有内容

function Remove-BuildFiles {
    Write-Info "Cleaning build files..."

    if (Test-Path "build") {
        Remove-Item -Path "build" -Recurse -Force
        Write-Success "Build directory deleted"
    } else {
        Write-Info "Build directory does not exist, skipping"
    }
}

# 清理输出文件函数 - 删除out目录及其所有内容

function Remove-OutputFiles {
    Write-Info "Cleaning output files..."

    if (Test-Path "out") {
        Remove-Item -Path "out" -Recurse -Force
        Write-Success "Output directory deleted"
    } else {
        Write-Info "Output directory does not exist, skipping"
    }
}

# 清理CMake缓存函数 - 删除CMake相关的缓存文件

function Remove-CMakeCache {
    Write-Info "Cleaning CMake cache..."

    if (Test-Path "CMakeCache.txt") {
        Remove-Item -Path "CMakeCache.txt" -Force
        Write-Success "CMakeCache.txt deleted"
    }

    if (Test-Path "CMakeFiles") {
        Remove-Item -Path "CMakeFiles" -Recurse -Force
        Write-Success "CMakeFiles directory deleted"
    }
}

# 清理IDE临时文件函数 - 删除各种IDE和编译产生的临时文件

function Remove-IDEtempFiles {
    Write-Info "Cleaning IDE temporary files..."

    # VSCode/Cursor临时文件清理
    if (Test-Path ".vscode") {
        Get-ChildItem -Path ".vscode" -Recurse -Include "*.log", "*.tmp" | Remove-Item -Force -ErrorAction SilentlyContinue
        Write-Info "VSCode temporary files cleaned"
    }

    # 编译产物文件清理 - 包括各种目标文件和库文件

    $tempPatterns = @("*.o", "*.obj", "*.exe", "*.dll", "*.so", "*.dylib", "*.a", "*.lib")

    foreach ($pattern in $tempPatterns) {
        Get-ChildItem -Path "." -Recurse -Include $pattern -ErrorAction SilentlyContinue | Remove-Item -Force -ErrorAction SilentlyContinue
    }

    Write-Info "Compilation product files cleaned"
}

# 显示清理结果函数 - 总结清理完成的内容

function Show-CleanResult {
    Write-Info "Cleanup completed!"
    Write-Info "Cleaned content:"
    Write-Info "  - Build directory (build)"
    Write-Info "  - Output directory (out)"
    Write-Info "  - CMake cache files"
    Write-Info "  - IDE temporary files"
    Write-Info "  - Compilation product files"

    Write-Success "Project restored to clean state, ready for rebuild"
}

# 主函数 - 执行完整的清理流程

function Main {
    Write-Info "Starting TFW project cleanup..."

    # 按顺序执行各个清理步骤

    Remove-BuildFiles
    Remove-OutputFiles
    Remove-CMakeCache
    Remove-IDEtempFiles
    Show-CleanResult

    Write-Success "TFW project cleanup completed!"
}

# 脚本入口点 - 判断是否直接运行脚本

if ($MyInvocation.InvocationName -ne '.') {
    # 如果脚本被直接运行（而不是点源加载），则执行主函数

    Main
}

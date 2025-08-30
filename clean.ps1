# TFW项目清理脚本 (Windows PowerShell)
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

# 清理构建文件
function Remove-BuildFiles {
    Write-Info "清理构建文件..."

    if (Test-Path "build") {
        Remove-Item -Path "build" -Recurse -Force
        Write-Success "已删除build目录"
    } else {
        Write-Info "build目录不存在，跳过"
    }
}

# 清理输出文件
function Remove-OutputFiles {
    Write-Info "清理输出文件..."

    if (Test-Path "out") {
        Remove-Item -Path "out" -Recurse -Force
        Write-Success "已删除out目录"
    } else {
        Write-Info "out目录不存在，跳过"
    }
}

# 清理CMake缓存
function Remove-CMakeCache {
    Write-Info "清理CMake缓存..."

    if (Test-Path "CMakeCache.txt") {
        Remove-Item -Path "CMakeCache.txt" -Force
        Write-Success "已删除CMakeCache.txt"
    }

    if (Test-Path "CMakeFiles") {
        Remove-Item -Path "CMakeFiles" -Recurse -Force
        Write-Success "已删除CMakeFiles目录"
    }
}

# 清理IDE临时文件
function Remove-IDEtempFiles {
    Write-Info "清理IDE临时文件..."

    # VSCode/Cursor临时文件
    if (Test-Path ".vscode") {
        Get-ChildItem -Path ".vscode" -Recurse -Include "*.log", "*.tmp" | Remove-Item -Force -ErrorAction SilentlyContinue
        Write-Info "已清理VSCode临时文件"
    }

    # 其他可能的临时文件
    $tempPatterns = @("*.o", "*.obj", "*.exe", "*.dll", "*.so", "*.dylib", "*.a", "*.lib")

    foreach ($pattern in $tempPatterns) {
        Get-ChildItem -Path "." -Recurse -Include $pattern -ErrorAction SilentlyContinue | Remove-Item -Force -ErrorAction SilentlyContinue
    }

    Write-Info "已清理编译产物文件"
}

# 显示清理结果
function Show-CleanResult {
    Write-Info "清理完成！"
    Write-Info "已清理的内容："
    Write-Info "  - 构建目录 (build\)"
    Write-Info "  - 输出目录 (out\)"
    Write-Info "  - CMake缓存文件"
    Write-Info "  - IDE临时文件"
    Write-Info "  - 编译产物文件"

    Write-Success "项目已恢复到干净状态，可以重新构建"
}

# 主函数
function Main {
    Write-Info "开始清理TFW项目..."

    Remove-BuildFiles
    Remove-OutputFiles
    Remove-CMakeCache
    Remove-IDEtempFiles
    Show-CleanResult

    Write-Success "TFW项目清理完成！"
}

# 脚本入口
if ($MyInvocation.InvocationName -ne '.') {
    Main
}

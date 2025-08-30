# TFW项目IDE配置指南

## 适用范围

本配置指南适用于以下开发环境：
- **操作系统**：Windows、Linux、macOS、WSL (Windows Subsystem for Linux)
- **编辑器**：VSCode、Cursor、VSCodium等基于VSCode的编辑器
- **项目类型**：C/C++项目，使用CMake构建系统
- **开发团队**：需要跨平台协作的团队

## 使用前提条件

### 系统要求
- **Windows**：Windows 10/11，支持WSL2或原生开发
- **Linux**：Ubuntu 18.04+、CentOS 7+、Debian 9+
- **macOS**：macOS 10.14+ (Mojave)
- **WSL**：WSL2 with Ubuntu 20.04+ 或 Debian 11+

### 必需工具
- **CMake**：3.16.0 或更高版本
- **编译器**：GCC 7+、Clang 6+、MSVC 2019+
- **Python**：3.6+ (用于跨平台脚本)
- **Git**：2.20+ (版本控制)

### 推荐工具
- **clang-format**：代码格式化
- **clang-tidy**：代码质量检查
- **Ninja**：快速构建工具

## 概述

本文档提供了在VSCode或Cursor中配置TFW项目的完整指南，包括代码格式化、质量检查、构建等核心功能的配置方法。调试功能作为可选功能提供简要说明。特别针对跨平台开发环境进行了优化，确保在Windows、Linux、macOS和WSL中都能正常工作。

## 兼容性说明

**重要**：本配置完全兼容VSCode和Cursor，支持跨平台开发！
- **编辑器兼容**：Cursor基于VSCode构建，所有配置都可以直接使用
- **跨平台支持**：Windows、Linux、macOS、WSL完全兼容
- **团队协作**：团队成员可以使用不同的操作系统和编辑器，配置保持一致
- **自动适配**：VSCode任务自动检测平台并使用相应的构建脚本

## 快速开始

### 1. 安装必要工具

#### 系统级工具安装

**Ubuntu/Debian (Linux/WSL)**
```bash
sudo apt update
sudo apt install -y clang-format clang-tidy cmake build-essential python3 python3-pip git
```

**macOS**
```bash
# 安装Homebrew (如果未安装)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# 安装开发工具
brew install llvm cmake python3 git
```

**Windows (原生开发)**
```cmd
# 安装Chocolatey (如果未安装)
Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))

# 安装开发工具
choco install cmake visualstudio2019buildtools git python
```

**Windows (WSL2)**
```bash
# 在WSL2中执行Linux安装命令
sudo apt update
sudo apt install -y clang-format clang-tidy cmake build-essential python3 python3-pip git
```

#### VSCode/Cursor扩展
推荐安装以下扩展：
- `ms-vscode.cmake-tools` - CMake项目管理
- `ms-vscode.cpptools` - C/C++开发支持
- `llvm-vs-code-extensions.vscode-clangd` - Clangd语言服务器
- `xaver.clang-format` - 代码格式化
- `ms-vscode.powershell` - Windows PowerShell支持

### 2. 项目配置

#### 基础配置步骤
1. **克隆项目**：将项目克隆到本地
2. **打开项目**：在VSCode/Cursor中打开项目根目录
3. **自动检测**：系统会自动检测CMake项目并提示配置
4. **选择编译器**：根据平台选择相应的编译器
5. **选择构建类型**：选择Debug或Release模式

#### 平台特定配置

**Linux/WSL/macOS**
- 编译器：GCC或Clang
- 生成器：Unix Makefiles
- 构建工具：make

**Windows (原生)**
- 编译器：MSVC (Visual Studio)
- 生成器：Visual Studio 16 2019
- 构建工具：MSBuild

**Windows (WSL2)**
- 编译器：GCC或Clang
- 生成器：Unix Makefiles
- 构建工具：make

#### 首次配置
```bash
# 创建必要的目录
mkdir -p build out/bin out/lib

# 进入构建目录（CMake需要在build目录中执行）
cd build

# 配置项目 (Linux/WSL/macOS)
# 注意：.. 指向根目录，因为CMakeLists.txt在根目录
cmake .. -DCMAKE_BUILD_TYPE=Release

# 配置项目 (Windows原生)
# 注意：.. 指向根目录，因为CMakeLists.txt在根目录
cmake .. -G "Visual Studio 16 2019" -A x64 -DCMAKE_BUILD_TYPE=Release

# 构建项目
cmake --build . --config Release
```

#### 重要说明
- **配置文件位置**：`CMakeLists.txt`等配置文件位于项目根目录
- **构建目录**：`build/`目录用于存放CMake生成的中间文件
- **脚本入口**：便捷的构建脚本（如`build_simple.sh`）位于根目录
- **构建过程**：需要进入`build`目录执行CMake命令，但CMake会读取根目录的配置文件

## 配置文件详解

### .vscode/settings.json

编辑器的基础配置，控制代码格式、保存行为等。支持跨平台自动适配。

```json
{
    // 编辑器基础设置
    "editor.tabSize": 4,                    // 设置Tab为4个空格
    "editor.insertSpaces": true,            // 插入空格而不是Tab
    "editor.detectIndentation": false,      // 禁用自动检测缩进
    "editor.rulers": [120],                // 在第120列显示标尺线
    "editor.formatOnSave": true,            // 保存时自动格式化
    "editor.formatOnPaste": true,           // 粘贴时自动格式化
    "editor.codeActionsOnSave": {           // 保存时自动执行的代码操作
        "source.fixAll": true,              // 修复所有可修复的问题
        "source.organizeImports": true      // 整理导入语句
    },

    // 文件处理设置
    "files.trimTrailingWhitespace": true,   // 自动删除行尾空格
    "files.insertFinalNewline": true,       // 文件末尾自动添加换行
    "files.trimFinalNewlines": true,        // 删除文件末尾多余的空行

    // C/C++特定设置
    "C_Cpp.default.cStandard": "c11",      // 设置C语言标准为C11
    "C_Cpp.default.cppStandard": "c++11",  // 设置C++标准为C++11
    "C_Cpp.default.intelliSenseMode": "linux-gcc-x64", // 智能感知模式

    // CMake设置 (跨平台自动适配)
    "cmake.configureOnOpen": true,          // 打开项目时自动配置CMake
    "cmake.buildDirectory": "${workspaceFolder}/build", // 构建目录
    "cmake.parallelJobs": 4,                // 并行构建任务数

    // 平台特定CMake设置
    "cmake.generator": "Unix Makefiles",    // Linux/WSL/macOS默认生成器
    "cmake.preferredGenerators": [          // 生成器优先级
        "Unix Makefiles",                   // Linux/WSL/macOS优先
        "Visual Studio 16 2019",            // Windows优先
        "Ninja"                             // 快速构建工具
    ],

    // 跨平台路径处理
    "files.eol": "\n",                      // 统一使用LF行尾符
    "files.encoding": "utf8",               // 统一使用UTF-8编码
    "terminal.integrated.defaultProfile.linux": "bash",    // Linux/WSL默认终端
    "terminal.integrated.defaultProfile.windows": "PowerShell" // Windows默认终端
}
```

### .vscode/extensions.json

推荐扩展列表，帮助团队成员安装必要的工具。

```json
{
    "recommendations": [
        // CMake工具链
        "ms-vscode.cmake-tools",            // CMake项目管理和构建
        "twxs.cmake",                       // CMake语法高亮和智能提示

        // C/C++开发工具
        "ms-vscode.cpptools",               // Microsoft C/C++扩展
        "ms-vscode.cpptools-extension-pack", // C/C++扩展包
        "ms-vscode.clangd",                 // Clangd语言服务器

        // 代码质量工具
        "llvm-vs-code-extensions.vscode-clangd", // LLVM Clangd扩展
        "ms-vscode.makefile-tools",         // Makefile工具支持

        // 代码格式化
        "xaver.clang-format",               // Clang-format格式化工具

        // 调试工具
        "ms-vscode.debug-auto-launch",      // 自动启动调试器
        "ms-vscode.debugger-for-chrome"     // Chrome调试器支持
    ]
}
```

### .clang-format

代码格式化规则，确保代码风格一致。

```yaml
---
# 基于Google风格，适配TFW编码规范
Language: Cpp
BasedOnStyle: Google

# 缩进设置
AccessModifierOffset: -4                   # 访问修饰符偏移量
IndentWidth: 4                             # 缩进宽度为4个空格
TabWidth: 4                                # Tab宽度为4个空格
UseTab: Never                              # 不使用Tab，只使用空格

# 大括号风格（K&R风格）
BraceWrapping:
  AfterClass: false                        # 类后不换行
  AfterControlStatement: false             # 控制语句后不换行
  AfterFunction: false                     # 函数后不换行
  AfterNamespace: false                    # 命名空间后不换行
  AfterStruct: false                       # 结构体后不换行
  AfterUnion: false                        # 联合体后不换行
  BeforeCatch: false                       # catch前不换行
  BeforeElse: false                        # else前不换行
  IndentBraces: false                      # 大括号不缩进
  SplitEmptyFunction: false                # 空函数不分割

# 行长度限制
ColumnLimit: 120                           # 最大行长度为120字符

# 函数和类设置
AllowShortFunctionsOnASingleLine: Empty    # 空函数允许单行
AllowShortIfStatementsOnASingleLine: false # if语句不允许单行
AllowShortLoopsOnASingleLine: false        # 循环不允许单行
AlwaysBreakAfterReturnType: None           # 返回类型后不强制换行

# 指针和对齐
PointerAlignment: Left                     # 指针符号靠左对齐
DerivePointerAlignment: false              # 不自动推导指针对齐

# 命名空间和模板
NamespaceIndentation: None                 # 命名空间不缩进
AlwaysBreakTemplateDeclarations: Yes       # 模板声明总是换行

# 注释设置
ReflowComments: true                       # 重新格式化注释
FixNamespaceComments: true                 # 修复命名空间注释

# 包含文件排序
SortIncludes: true                         # 排序包含文件
SortUsingDeclarations: true                # 排序using声明
```

### .clang-tidy

代码质量检查规则，发现潜在问题和改进建议。

```yaml
---
# 启用所有检查，排除不相关的检查
Checks: >
  *,
  -fuchsia-*,
  -google-*,
  -zircon-*,
  -abseil-*,
  -llvm-*,
  -llvmlibc-*,
  -modernize-use-trailing-return-type

# 检查选项配置
CheckOptions:
  # 命名规范检查
  - key: readability-identifier-naming.ClassCase
    value: CamelCase                      # 类名使用PascalCase
  - key: readability-identifier-naming.FunctionCase
    value: lower_case                     # 函数名使用snake_case
  - key: readability-identifier-naming.VariableCase
    value: lower_case                     # 变量名使用snake_case
  - key: readability-identifier-naming.ParameterCase
    value: lower_case                     # 参数名使用snake_case
  - key: readability-identifier-naming.ConstantCase
    value: UPPER_CASE                     # 常量名使用UPPER_CASE
  - key: readability-identifier-naming.MacroDefinitionCase
    value: UPPER_CASE                     # 宏定义使用UPPER_CASE

  # 其他检查选项
  - key: readability-braces-around-statements.ShortStatementLines
    value: 1                              # 短语句行数阈值
  - key: performance-unnecessary-value-param.AllowedTypes
    value: ""                             # 允许的值参数类型
```

### .vscode/tasks.json

跨平台任务配置，自动检测平台并使用相应的构建脚本。支持Windows、Linux、macOS和WSL。

```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "构建项目",
            "type": "shell",
            "group": "build",
            "presentation": {
                "echo": true,              // 显示执行的命令
                "reveal": "always",        // 总是显示终端
                "focus": false,            // 不聚焦到终端
                "panel": "shared"          // 使用共享终端面板
            },
            "problemMatcher": ["$gcc"],    // 使用GCC问题匹配器
            "detail": "使用CMake构建整个项目",

            // 跨平台命令配置
            "windows": {
                "command": "powershell.exe",
                "args": ["-ExecutionPolicy", "Bypass", "-File", "${workspaceFolder}/build_simple.ps1"]
            },
            "linux": {
                "command": "bash",
                "args": ["${workspaceFolder}/build_simple.sh"]
            },
            "osx": {
                "command": "bash",
                "args": ["${workspaceFolder}/build_simple.sh"]
            }
        },
        {
            "label": "清理项目",
            "type": "shell",
            "group": "build",
            "detail": "清理所有构建文件和输出文件",

            // 跨平台清理命令
            "windows": {
                "command": "powershell.exe",
                "args": ["-ExecutionPolicy", "Bypass", "-File", "${workspaceFolder}/clean.ps1"]
            },
            "linux": {
                "command": "bash",
                "args": ["${workspaceFolder}/clean.sh"]
            },
            "osx": {
                "command": "bash",
                "args": ["${workspaceFolder}/clean.sh"]
            }
        },
        {
            "label": "检查代码格式",
            "type": "shell",
            "group": "test",
            "detail": "检查代码格式，不修改文件（安全检查）",

            // 跨平台格式检查命令（不修改文件）
            "windows": {
                "command": "powershell.exe",
                "args": [
                    "-Command",
                    "Get-ChildItem -Recurse -Include *.c,*.cpp,*.h,*.hpp | ForEach-Object { clang-format $_.FullName }"
                ]
            },
            "linux": {
                "command": "find",
                "args": [".", "-name", "*.c", "-o", "-name", "*.cpp", "-o", "-name", "*.h", "-o", "-name", "*.hpp", "-exec", "clang-format", "{}", "+"]
            },
            "osx": {
                "command": "find",
                "args": [".", "-name", "*.c", "-o", "-name", "*.cpp", "-o", "-name", "*.h", "-o", "-name", "*.hpp", "-exec", "clang-format", "{}", "+"]
            }
        },
        {
            "label": "格式化代码（会修改文件）",
            "type": "shell",
            "group": "build",
            "detail": "使用clang-format格式化所有C/C++文件（⚠️ 会直接修改源文件）",

            // 跨平台格式化命令（会修改文件）
            "windows": {
                "command": "powershell.exe",
                "args": [
                    "-Command",
                    "Get-ChildItem -Recurse -Include *.c,*.cpp,*.h,*.hpp | ForEach-Object { clang-format -i $_.FullName }"
                ]
            },
            "linux": {
                "command": "find",
                "args": [".", "-name", "*.c", "-o", "-name", "*.cpp", "-o", "-name", "*.h", "-o", "-name", "*.hpp", "-exec", "clang-format", "-i", "{}", "+"]
            },
            "osx": {
                "command": "find",
                "args": [".", "-name", "*.c", "-o", "-name", "*.cpp", "-o", "-name", "*.h", "-o", "-name", "*.hpp", "-exec", "clang-format", "-i", "{}", "+"]
            }
        },
        {
            "label": "代码质量检查",
            "type": "shell",
            "group": "test",
            "detail": "使用clang-tidy检查代码质量",

            "windows": {
                "command": "powershell.exe",
                "args": [
                    "-Command",
                    "Get-ChildItem -Recurse -Include *.c,*.cpp,*.h,*.hpp | ForEach-Object { clang-tidy -p build/compile_commands.json $_.FullName }"
                ]
            },
            "linux": {
                "command": "find",
                "args": [
                    ".",
                    "-name", "*.c", "-o",
                    "-name", "*.cpp", "-o",
                    "-name", "*.h", "-o",
                    "-name", "*.hpp",
                    "-exec", "clang-tidy", "-p", "build/compile_commands.json", "{}", "+"
                ]
            },
            "osx": {
                "command": "find",
                "args": [
                    ".",
                    "-name", "*.c", "-o",
                    "-name", "*.cpp", "-o",
                    "-name", "*.h", "-o",
                    "-name", "*.hpp",
                    "-exec", "clang-tidy", "-p", "build/compile_commands.json", "{}", "+"
                ]
            }
        },
        {
            "label": "生成编译数据库",
            "type": "shell",
            "group": "build",
            "detail": "生成compile_commands.json文件供IDE使用",

            // 跨平台CMake命令
            "windows": {
                "command": "cmake",
                "args": ["..", "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"],
                "options": {
                    "cwd": "${workspaceFolder}/build"
                }
            },
            "linux": {
                "command": "cmake",
                "args": ["..", "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"],
                "options": {
                    "cwd": "${workspaceFolder}/build"
                }
            },
            "osx": {
                "command": "cmake",
                "args": ["..", "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"],
                "options": {
                    "cwd": "${workspaceFolder}/build"
                }
            }
        },
        {
            "label": "跨平台构建 (Python)",
            "type": "shell",
            "command": "python3",
            "args": ["${workspaceFolder}/build.py"],
            "group": "build",
            "detail": "使用Python脚本进行跨平台构建",
            "options": {
                "cwd": "${workspaceFolder}"
            }
        }
    ]
}
```

### .vscode/launch.json (可选)

调试配置文件，支持程序调试和断点设置。**注意：此文件为可选配置，仅在需要调试功能时创建。**

**功能说明**：
- 支持C/C++程序调试
- 支持断点设置和单步执行
- 支持变量查看和内存检查
- 跨平台调试器支持（Windows/Linux用GDB，macOS用LLDB）

**创建方法**：
1. 在`.vscode`目录下创建`launch.json`
2. 配置可执行文件路径和调试参数
3. 选择对应的调试器类型

**使用场景**：
- 开发阶段需要调试可执行文件
- 需要详细的程序执行跟踪
- 问题排查和性能分析

**注意事项**：
- 需要先配置构建任务
- 需要正确的调试器环境
- 建议在项目相对稳定后再配置

## 使用方法

### 快捷键

| 功能 | 快捷键 | 说明 |
|------|--------|------|
| 格式化代码 | `Shift + Alt + F` | 格式化当前文件 |
| 构建项目 | `Ctrl + Shift + P` → "Tasks: Run Task" → "构建项目" | 运行构建任务 |
| 清理项目 | `Ctrl + Shift + P` → "Tasks: Run Task" → "清理项目" | 运行清理任务 |
| 调试程序 | `F5` | 启动调试器 (需要launch.json) |
| 设置断点 | `F9` | 在当前行设置/取消断点 (需要launch.json) |

### 常用命令

#### 命令面板（Ctrl + Shift + P）
- `CMake: Configure` - 配置CMake项目
- `CMake: Build` - 构建项目
- `CMake: Clean` - 清理构建
- `C/C++: Edit Configurations (UI)` - 编辑C/C++配置

#### 终端命令
```bash
# 构建项目
./build_simple.sh

# 清理项目
./clean.sh

# 格式化代码
find . -name '*.c' -o -name '*.cpp' -o -name '*.h' -o -name '*.hpp' | xargs clang-format -i

# 代码质量检查
clang-tidy -p build/compile_commands.json
```

## 故障排除

### 常见问题

#### 1. 头文件找不到
**症状**：红色波浪线，智能提示不工作
**解决方案**：
1. 确保生成了`compile_commands.json`
2. 运行"生成编译数据库"任务
3. 重启VSCode/Cursor

#### 2. 格式化不生效
**症状**：保存时没有自动格式化
**解决方案**：
1. 检查`.clang-format`文件是否存在
2. 确认安装了clang-format扩展
3. 检查`settings.json`中的格式化设置

#### 3. CMake配置失败
**症状**：CMake配置错误
**解决方案**：
1. 检查CMake版本（需要3.16+）
2. 确认安装了必要的构建工具
3. 检查CMakeLists.txt语法

#### 4. 调试器不工作 (可选功能)
**症状**：无法启动调试器
**解决方案**：
1. 确认程序已构建成功
2. 检查`launch.json`中的程序路径
3. 确认安装了GDB或LLDB

**注意**：调试功能为可选功能，如果不需要可以跳过此步骤。

### 调试技巧 (可选功能)

#### 查看编译输出
1. 打开终端（Ctrl + `）
2. 运行构建命令
3. 查看详细错误信息

#### 检查配置
1. 查看状态栏的CMake信息
2. 检查`.vscode`目录下的配置文件
3. 确认所有必要的扩展已安装

## 最佳实践

### 开发流程
1. **开始工作前**：运行"构建项目"确保代码能编译
2. **编写代码时**：保存时自动格式化，保持代码风格一致
3. **提交前**：运行"代码质量检查"，修复发现的问题
4. **定期维护**：清理构建文件，保持项目整洁

### 团队协作
1. **统一配置**：所有团队成员使用相同的配置文件
2. **版本控制**：将`.vscode`目录和配置文件纳入版本控制
3. **文档更新**：配置变更时及时更新本文档
4. **培训支持**：新成员加入时提供配置指导

### 性能优化
1. **并行构建**：在`settings.json`中设置合适的并行任务数
2. **增量构建**：利用CMake的增量构建特性
3. **缓存优化**：合理使用构建缓存，避免重复编译

## 总结

通过本配置指南，您可以在VSCode或Cursor中获得：
- ✅ 自动代码格式化
- ✅ 智能代码提示
- ✅ 实时错误检查
- ✅ 便捷的构建和调试 (调试为可选功能)
- ✅ 统一的代码风格
- ✅ 高效的开发体验

这些配置完全兼容VSCode和Cursor，确保团队成员可以使用不同的编辑器而保持一致的开发体验。

## 跨平台脚本说明

### 脚本文件结构
```
project_root/
├── build_simple.sh          # Linux/macOS/WSL构建脚本
├── build_simple.bat         # Windows批处理构建脚本
├── build_simple.ps1         # Windows PowerShell构建脚本
├── clean.sh                 # Linux/macOS/WSL清理脚本
├── clean.bat                # Windows批处理清理脚本
├── clean.ps1                # Windows PowerShell清理脚本
├── build.py                 # 跨平台Python构建脚本（推荐）
└── .vscode/
    ├── settings.json        # 跨平台编辑器设置
    ├── tasks.json           # 跨平台任务配置
    └── launch.json          # 调试配置 (可选)
```

### 脚本选择建议

**推荐使用Python脚本**：
- 跨平台兼容性最好
- 维护成本最低
- 功能最完整

**平台特定脚本**：
- 性能更好
- 与平台集成更紧密
- 适合高级用户

### 脚本权限设置

**Linux/macOS/WSL**：
```bash
chmod +x build_simple.sh clean.sh
```

**Windows**：
- 批处理文件无需特殊权限
- PowerShell脚本可能需要调整执行策略：
```powershell
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
```

### 故障排除

**WSL用户**：
- 确保在WSL环境中运行Linux脚本
- 检查文件权限和行尾符

**Windows用户**：
- 确保PowerShell执行策略允许运行脚本
- 检查CMake和编译器是否正确安装

**跨平台问题**：
- 使用Python脚本作为备选方案
- 检查VSCode任务配置是否正确
- 确认所有必要的工具都已安装

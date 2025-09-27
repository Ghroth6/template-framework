# TFW项目开发指南

## 1. 项目概述

### 1.1 项目简介

TFW（Template Framework）是一个现代化的C/C++开发框架，旨在提供一套跨平台、模块化的开发基础架构。该项目通过清晰的模块划分和良好的架构设计，帮助开发者快速构建稳定、可维护的应用程序。

### 1.2 技术栈

- **编程语言**：C/C++
- **构建系统**：CMake 3.20+
- **版本控制**：Git（含Submodule支持）
- **跨平台支持**：Windows、Linux、macOS

### 1.3 项目架构概览

项目采用分层模块化设计，主要包括以下几个核心模块：

1. **core模块**：核心功能模块，提供框架的主要业务逻辑
2. **utils模块**：工具模块，封装平台相关操作和通用工具函数
3. **cli模块**：命令行接口模块，提供命令行工具支持
4. **third_party模块**：第三方库管理模块，通过Git Submodule管理依赖

## 2. 开发环境搭建

### 2.1 系统要求

- **Windows**：Windows 10/11（支持WSL2）
- **Linux**：Ubuntu 18.04+、CentOS 7+、Debian 9+
- **macOS**：macOS 10.14+ (Mojave)

### 2.2 必需工具安装

#### 2.2.1 Windows

```cmd
# 安装Chocolatey (如果未安装)
Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))

# 安装开发工具
choco install cmake git
```

2.2.2 Linux

```bash
# Ubuntu/Debian

sudo apt update
sudo apt install -y cmake build-essential git

# CentOS/RHEL

sudo yum install -y cmake gcc gcc-c++ make git
```

2.2.3 macOS

```bash
# 安装Homebrew (如果未安装)

/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# 安装开发工具

brew install cmake git
```

### 2.3 获取项目代码

```bash
# 克隆项目（包含submodule）

git clone --recurse-submodules https://github.com/Ghroth6/template-framework.git

# 如果已克隆但未初始化submodule

git submodule update --init --recursive
```

## 3. 项目结构与构建系统

### 3.1 目录结构概述

```bash
template-framework/
├── cli/                 # 命令行接口模块
├── core/                # 核心功能模块
├── docs/                # 文档目录
├── interface/           # 接口定义目录
├── runtime/             # 运行时相关文件
├── third_party/         # 第三方库目录
├── utils/               # 工具模块
├── build/               # 构建输出目录
├── out/                 # 最终输出目录
├── .git/                # Git版本控制目录
├── .gitmodules          # Git Submodule配置文件
├── CMakeLists.txt       # 项目根CMake配置
├── build_simple.sh      # 简单构建脚本（Linux/macOS）
├── build_simple.ps1     # 简单构建脚本（Windows）
├── clean.sh             # 清理脚本（Linux/macOS）
└── clean.ps1            # 清理脚本（Windows）
```

### 3.2 构建系统配置

#### 3.2.1 CMake配置概述

项目使用CMake作为主要构建系统，采用现代CMake实践，通过target-based方法管理依赖关系。

#### 3.2.2 CMake版本要求

项目要求使用CMake 3.20或更高版本，确保对现代CMake特性的支持。

#### 3.2.3 C/C++标准配置

C++标准：C++17（在根目录CMakeLists.txt中统一配置）
C标准：C11（在根目录CMakeLists.txt中统一配置）
这些标准在项目根目录统一设置，各子模块无需重复配置。

#### 3.2.4 模块依赖管理

##### 3.2.4.1 依赖传播规则

项目遵循CMake的依赖传播规则：

PUBLIC依赖：当模块A需要将依赖模块B的功能传递给链接A的上层模块时使用
PRIVATE依赖：当模块A仅内部使用依赖模块B，不希望上层模块知道B的存在时使用
INTERFACE依赖：当模块A作为接口库，自身不实现功能，但需要将依赖传递给上层模块时使用

##### 3.2.4.2 平台依赖管理

所有平台相关依赖应集中管理在utils模块中，其他模块通过链接utils模块获得平台功能，避免在多个模块中重复配置相同的平台依赖。

### 3.3 构建脚本说明

#### 3.3.1 标准构建脚本

项目提供跨平台构建脚本：

Linux/macOS：./build_simple.sh
Windows：.\build_simple.ps1

#### 3.3.2 清理脚本

项目提供跨平台清理脚本：

Linux/macOS：./clean.sh
Windows：.\clean.ps1

#### 3.3.3 跨平台支持

构建脚本自动检测平台并选择合适的构建工具，确保在不同操作系统上的一致性体验。

## 4. 代码规范

### 4.1 命名规范

#### 4.1.1 文件命名

头文件：TFW_模块名.h
源文件：TFW_模块名.c 或 TFW_模块名.cpp

#### 4.1.2 函数命名

C接口函数：TFW_函数名（大驼峰）
C++类方法：PascalCase（首字母大写的驼峰命名）
内部函数：PascalCase（首字母大写的驼峰命名）

#### 4.1.3 变量命名

全局/静态变量：g_变量名（小驼峰）
局部变量：小驼峰
成员变量：小驼峰_（下划线后缀）

#### 4.1.4 常量和宏命名

宏定义：TFW_模块_功能_具体名称（全大写，下划线分隔）
枚举常量：TFW_枚举类型_具体值

#### 4.1.5 类型定义

类型定义：TFW_类型名

#### 4.1.6 命名长度和可读性

1. 优先保证语义清晰：命名应能清楚表达其用途和含义，避免使用含糊不清的缩写
2. 适度控制长度：在语义清晰的前提下，避免过长的命名，建议控制在合理范围内
3. 缩写使用原则：
   常见缩写可以使用，如 Msg(消息)、Cfg(配置)、Mgr(管理器)等
   项目内应保持缩写一致性
   避免过度缩写导致语义不清晰

### 4.2 代码格式

#### 4.2.1 缩进和空格

使用4个空格进行缩进，不使用Tab
运算符前后添加空格
逗号后添加空格
函数调用的括号内侧不加空格

#### 4.2.2 大括号风格

使用K&R风格（首大括号与入口在同一行）
即使只有一行代码，也要使用大括号

#### 4.2.3 行长度

每行代码不建议超过120个字符

### 4.3 注释规范

使用Doxygen风格进行注释，包含函数参数、返回值、函数描述、变量描述、枚举值描述等

### 4.4 错误处理

使用统一错误码，并进行必要的返回值检查
错误分支中需要确认是否有资源需要释放

### 4.5 语言标准

C接口必须完全兼容C11标准
C++代码使用C++17标准

## 5. 版本控制

### 5.1 Git基础配置

#### 5.1.1 .gitignore说明

项目通过.gitignore文件控制版本控制范围：

忽略构建目录：build/和out/
忽略编译产物：*.o、*.obj、*.exe等
忽略IDE配置：.idea/、.cursor/等用户特定配置

#### 5.1.2 .gitattributes说明

项目通过.gitattributes文件确保跨平台兼容性：

统一使用LF行尾符
确保文本文件的正确处理
支持不同平台的文件属性设置

### 5.2 Submodule配置与管理

项目使用Git Submodule管理第三方库依赖，遵循以下原则：

第三方库通过Submodule方式集成
Submodule指向特定提交，确保版本一致性
主项目只保存Submodule的引用，不包含实际代码

### 5.3 分支管理策略

（待完善）

### 5.4 提交规范

（待完善）

## 6. 构建与部署

### 6.1 构建流程

进入项目根目录
执行构建脚本或手动执行CMake命令
构建产物输出到out/目录

### 6.2 输出目录结构

```bash
out/
├── bin/                 # 可执行文件
├── lib/                 # 库文件
└── include/             # 头文件
```

### 6.3 发布流程

（待完善）

## 7. 常见问题与故障排除

### 7.1 环境搭建问题

CMake版本不足
问题：提示CMake版本不足3.20 解决：升级CMake到3.20或更高版本

编译器不支持
问题：编译器不支持C++17或C11 解决：升级编译器或安装支持的编译器版本

### 7.2 构建问题

Submodule未初始化
问题：提示找不到third_party中的库 解决：执行git submodule update --init --recursive

平台库链接失败
问题：提示找不到pthread、rt等库 解决：检查平台相关依赖是否正确配置

### 7.3 运行时问题

（待完善）

## 附录

A. 工具链参考
（待完善）

B. CMake参考
（待完善）

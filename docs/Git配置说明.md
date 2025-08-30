# TFW项目Git配置说明

## 概述

本文档说明了TFW项目的Git配置文件（`.gitignore`和`.gitattributes`）的配置原理、使用方法和最佳实践。

## .gitignore 配置说明

### 配置原则

基于编码规范9.4节"编译流程控制"的要求：
- 编译生成的中间文件控制在独立的`build`目录下
- 最终生成的文件默认生成在`out`目录下
- 项目的主编译配置文件与入口位于根目录

### 主要忽略内容

#### 1. 构建目录
```
build/                    # CMake中间文件目录
out/                      # 最终输出文件目录
```
**原因**：这些目录包含编译产物，不应该纳入版本控制

#### 2. CMake生成文件
```
CMakeCache.txt
CMakeFiles/
cmake_install.cmake
Makefile
*.cmake
```
**原因**：这些是构建系统自动生成的文件，每次构建都会重新生成

#### 3. 编译产物
```
*.o, *.obj              # 目标文件
*.exe, *.dll, *.so      # 可执行文件和动态库
*.a, *.lib              # 静态库
```
**原因**：编译产物依赖于平台和编译器，不应该版本控制

#### 4. IDE配置文件
```
.vscode/                 # VSCode配置（建议纳入版本控制）
.idea/                   # IntelliJ IDEA配置
.cursor/                 # Cursor配置
```
**注意**：`.vscode/`目录建议纳入版本控制，确保团队配置一致

#### 5. 平台特定文件
```
*.vcxproj               # Visual Studio项目文件
*.sln                   # Visual Studio解决方案
*.suo                   # Visual Studio用户选项
```
**原因**：这些文件包含用户特定的配置，不应该共享

### 选择性忽略

#### compile_commands.json
```
compile_commands.json     # IDE编译数据库（可选择性忽略）
```
**说明**：
- 如果团队使用相同的开发环境，可以纳入版本控制
- 如果环境差异较大，建议忽略

## .gitattributes 配置说明

### 配置原则

基于编码规范和配置指南的要求：
- 支持跨平台开发（Windows、Linux、macOS、WSL）
- 确保代码风格一致
- 遵循命名规范和格式要求

### 主要配置内容

#### 1. 全局设置
```
* text=auto eol=lf
```
**作用**：
- `text=auto`：Git自动识别文本文件
- `eol=lf`：统一使用LF行尾符

#### 2. 源代码文件
```
*.h, *.hpp, *.c, *.cpp text eol=lf working-tree-encoding=UTF-8
```
**作用**：
- 遵循编码规范2.1-2.5节的命名规范
- 确保跨平台编码一致性

#### 3. 脚本文件
```
*.sh text eol=lf working-tree-encoding=UTF-8      # Linux/macOS/WSL
*.bat, *.cmd text eol=lf working-tree-encoding=UTF-8  # Windows
*.ps1 text eol=lf working-tree-encoding=UTF-8     # PowerShell
*.py text eol=lf working-tree-encoding=UTF-8      # Python
```
**作用**：
- 支持跨平台构建（编码规范9.4节）
- 确保脚本在不同平台正常工作

#### 4. IDE配置文件
```
.vscode/* text eol=lf working-tree-encoding=UTF-8
.clang-format text eol=lf working-tree-encoding=UTF-8
.clang-tidy text eol=lf working-tree-encoding=UTF-8
```
**作用**：
- 支持跨平台开发环境
- 确保代码格式化规则一致

#### 5. 平台特定文件
```
*.vcxproj text eol=crlf working-tree-encoding=UTF-8  # Windows
*.sln text eol=crlf working-tree-encoding=UTF-8       # Windows
```
**作用**：
- Windows项目文件使用CRLF行尾符
- 确保Visual Studio兼容性

## 使用建议

### 团队协作

1. **统一配置**：所有团队成员使用相同的Git配置
2. **版本控制**：将`.gitignore`和`.gitattributes`纳入版本控制
3. **定期更新**：根据项目发展更新配置

### 跨平台开发

1. **Windows用户**：Git自动处理行尾符转换
2. **Linux/macOS/WSL用户**：保持LF行尾符
3. **编辑器配置**：使用统一的编码和行尾符设置

### 最佳实践

1. **构建目录**：始终忽略`build/`和`out/`目录
2. **IDE配置**：选择性纳入`.vscode/`目录
3. **平台文件**：忽略用户特定的配置文件
4. **编码一致**：使用UTF-8编码和LF行尾符

## 故障排除

### 常见问题

#### 1. 行尾符问题
**症状**：文件显示有大量行尾符差异
**解决方案**：
```bash
git config --global core.autocrlf true  # Windows
git config --global core.autocrlf input # Linux/macOS
```

#### 2. 编码问题
**症状**：中文显示乱码
**解决方案**：
```bash
git config --global core.quotepath false
git config --global gui.encoding utf-8
```

#### 3. 文件权限问题
**症状**：脚本文件无法执行
**解决方案**：
```bash
git update-index --chmod=+x script.sh
```

### 配置验证

#### 检查当前配置
```bash
# 查看Git配置
git config --list | grep -E "(core\.|gui\.)"

# 查看文件属性
git check-attr -a filename

# 查看行尾符
file filename
```

#### 测试跨平台兼容性
1. 在不同操作系统上克隆项目
2. 检查文件编码和行尾符
3. 验证构建脚本是否正常工作

## 总结

TFW项目的Git配置确保了：
- ✅ 跨平台开发兼容性
- ✅ 代码风格一致性
- ✅ 团队协作效率
- ✅ 构建环境整洁性

通过合理的`.gitignore`和`.gitattributes`配置，团队成员可以在Windows、Linux、macOS和WSL等不同环境中享受一致的开发体验。

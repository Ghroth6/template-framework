# ============================================================================
# TFW Project Main CMake Configuration
# TFW 项目主构建配置
# ============================================================================
#
# this file is used to configure the compilation of the TFW project
# 本文件用于配置 TFW 项目的编译
# it should conform to the TFW project coding standard requirements
# 它应符合 TFW 项目编码规范要求
#
# ============================================================================
#
# Compilation Instructions:
# 编译说明：
#
# 1. normal compilation: execute from the project root directory
#    1. 标准编译：在项目根目录下执行
#    cd /path/to/template-framework
#    ./build_simple.sh
#
# 2. separate compilation (only needed for development and debugging):
#    2. 单独编译（主要用于开发和调试）：
#    cd /path/to/template-framework
#    mkdir -p build && cd build
#    cmake .. -DCMAKE_BUILD_TYPE=Debug
#    make -j$(nproc)
#
# note: separate compilation is mainly used for development and debugging, please compile from the root directory for normal use
# 注意：单独编译主要用于开发和调试，正常使用时请从根目录进行编译
#
# ============================================================================

cmake_minimum_required(VERSION 3.20)
project(TFW VERSION 1.0.0 LANGUAGES C CXX)

# 定义项目版本变量，供子模块使用
set(TFW_VERSION_MAJOR ${CMAKE_PROJECT_VERSION_MAJOR})
set(TFW_VERSION_MINOR ${CMAKE_PROJECT_VERSION_MINOR})
set(TFW_VERSION_PATCH ${CMAKE_PROJECT_VERSION_PATCH})
set(TFW_VERSION_STRING ${CMAKE_PROJECT_VERSION})

# 收集构建信息
string(TIMESTAMP TFW_BUILD_TIMESTAMP "%Y-%m-%d %H:%M:%S")

# 获取Git信息（如果在Git仓库中）
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${CMAKE_SOURCE_DIR}/.git")
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE TFW_GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE TFW_GIT_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    execute_process(
        COMMAND ${GIT_EXECUTABLE} diff-index --quiet HEAD --
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        RESULT_VARIABLE TFW_GIT_DIRTY
    )

    if(TFW_GIT_DIRTY EQUAL 0)
        set(TFW_GIT_DIRTY "false")
    else()
        set(TFW_GIT_DIRTY "true")
    endif()
else()
    set(TFW_GIT_COMMIT_HASH "unknown")
    set(TFW_GIT_BRANCH "unknown")
    set(TFW_GIT_DIRTY "false")
endif()

# 编译器和平台信息
set(TFW_COMPILER_ID ${CMAKE_CXX_COMPILER_ID})
set(TFW_COMPILER_VERSION ${CMAKE_CXX_COMPILER_VERSION})
set(TFW_BUILD_TYPE ${CMAKE_BUILD_TYPE})

if(WIN32)
    set(TFW_TARGET_PLATFORM "Windows")
elseif(APPLE)
    set(TFW_TARGET_PLATFORM "macOS")
else()
    set(TFW_TARGET_PLATFORM "Linux")
endif()

set(TFW_TARGET_ARCH ${CMAKE_SYSTEM_PROCESSOR})
# ============================================================================
# build options configuration
# 构建选项配置
# ============================================================================

# 开发时使用静态库，发布时使用动态库
# When developing, use static libraries, when releasing, use dynamic libraries
option(BUILD_SHARED_LIBS "Build static libraries when developing" OFF)
# option(BUILD_SHARED_LIBS "Build shared libraries when releasing" ON)

# 设置MSVC编译器使用UTF-8编码，解决中文注释编译问题
# Set MSVC compiler to use UTF-8 encoding to resolve Chinese comment compilation issues
if(MSVC)
    add_compile_options(/utf-8)

    # 确保正确链接C++标准库
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")

    # 设置运行时库类型
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

    # 确保C++标准库正确链接
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus")

    message(STATUS "MSVC UTF-8 encoding enabled for ${PROJECT_NAME}")
endif()

# ============================================================================
# C/C++ standard configuration
# C/C++ 标准配置
# ============================================================================

# Set C/C++ standard
# 设置 C/C++ 标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================================================
# shared library specific settings
# 动态库特定设置
# ============================================================================

if(BUILD_SHARED_LIBS)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    if(WIN32)
        set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    elseif(APPLE)
        # macOS平台全局动态库设置
        set(CMAKE_INSTALL_NAME_DIR "@rpath")
        set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
    else()
        # Linux/Unix平台全局动态库设置
        set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
        set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    endif()
    message(STATUS "Building shared libraries (dynamic linking)")
else()
    message(STATUS "Building static libraries")
endif()

# ============================================================================
# output directory structure configuration
# 输出目录结构配置
# ============================================================================

# Set output directory structure (coding standard 9.4 section)
# 设置输出目录结构（编码标准 9.4 节）
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/lib)

# ============================================================================
# IDE support configuration
# IDE 支持配置
# ============================================================================

# Generate compile_commands.json (for IDE support)
# 生成 compile_commands.json（用于 IDE 支持）
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ============================================================================
# platform information display
# 平台信息显示
# ============================================================================

# display platform information
# 显示平台信息
if(WIN32)
    message(STATUS "Target platform: Windows")
elseif(APPLE)
    message(STATUS "Target platform: macOS")
else()
    message(STATUS "Target platform: Linux/Unix")
endif()

# ============================================================================
# subdirectory configuration
# 子目录配置
# ============================================================================

# 添加子目录
# Add subdirectory
# if(EXISTS "${CMAKE_SOURCE_DIR}/utils/CMakeLists.txt")
#     add_subdirectory(utils)
#     message(STATUS "Utils module has been configured")
# endif()

# 添加第三方库管理
# Add third party libraries management
if(EXISTS "${CMAKE_SOURCE_DIR}/third_party/CMakeLists.txt")
    add_subdirectory(third_party)
    message(STATUS "Third party libraries management has been configured")
else()
    message(STATUS "Third party libraries management does not exist, skip configuration")
endif()

# 添加utils模块
# Add utils module
if(EXISTS "${CMAKE_SOURCE_DIR}/utils/CMakeLists.txt")
    add_subdirectory(utils)
    message(STATUS "Utils module has been configured")
else()
    message(STATUS "Utils module configuration does not exist, skip configuration")
endif()

# 添加core模块
# Add core module
if(EXISTS "${CMAKE_SOURCE_DIR}/core/CMakeLists.txt")
    add_subdirectory(core)
    message(STATUS "Core module has been configured")
else()
    message(STATUS "Core module configuration does not exist, skip configuration")
endif()

# ============================================================================
# example program configuration
# 示例程序配置
# ============================================================================
#
# 示例程序配置，可以根据实际需要调整
# Example program configuration (optional, adjust according to actual needs)
#

# 添加CLI模块
# Add CLI module
if(EXISTS "${CMAKE_SOURCE_DIR}/cli/CMakeLists.txt")
    add_subdirectory(cli)
    message(STATUS "CLI module has been configured")
else()
    message(STATUS "CLI module configuration does not exist, skip configuration")
endif()

# ============================================================================
# test program configuration
# 测试程序配置
# ============================================================================

# Test program configuration (optional)
# 测试程序配置（可选）
# 注意：需要根据实际测试文件调整路径
# Note: Need to adjust the path according to the actual test files
if(EXISTS "${CMAKE_SOURCE_DIR}/tests/CMakeLists.txt")
    enable_testing()
    add_subdirectory(tests)
    message(STATUS "Test program has been configured")
else()
    message(STATUS "Test program configuration does not exist, skip configuration")
endif()


# ============================================================================
# installation configuration
# 安装配置
# ============================================================================
#
# 注意：以下安装配置在项目初期通常不需要，建议在以下情况时再启用：
# Note: The following installation configuration is usually not needed in the early stages of the project,
# it is recommended to enable it in the following situations:
# 1. 项目相对稳定，API接口基本确定
#    1. The project is relatively stable and the API interface is basically determined
# 2. 需要发布到包管理器或供其他项目使用
#    2. Need to publish to package manager or for use by other projects
# 3. 团队协作需要统一的库安装方式
#    3. Team collaboration requires a unified library installation method
#
# 启用方法：
# Enable method:
# 1. 取消注释下面的install命令
#    1. Uncomment the install commands below
# 2. 根据实际需求调整安装路径和配置
#    2. Adjust the installation path and configuration according to actual needs
# 3. 测试安装和卸载流程
#    3. Test the installation and uninstallation process
#
# 示例配置（需要时取消注释并调整）：
# Example configuration (uncomment and adjust when needed):
#
# # 安装头文件
# # Install header files
# install(DIRECTORY interface/
#     DESTINATION include/tfw
#     FILES_MATCHING PATTERN "*.h"
# )
#
# # 安装库文件
# # Install library files
# if(BUILD_SHARED_LIBS)
#     # 动态库安装配置
#     # Shared library installation configuration
#     install(TARGETS TFW_core TFW_utils
#         EXPORT TFWTargets
#         LIBRARY DESTINATION lib
#         ARCHIVE DESTINATION lib
#         RUNTIME DESTINATION bin
#         INCLUDES DESTINATION include
#     )
# else()
#     # 静态库安装配置
#     # Static library installation configuration
#     install(TARGETS TFW_core TFW_utils
#         EXPORT TFWTargets
#         ARCHIVE DESTINATION lib
#         INCLUDES DESTINATION include
#     )
# endif()
#
# # 安装CMake配置文件
# # Install CMake configuration files
# install(EXPORT TFWTargets
#     FILE TFWTargets.cmake
#     NAMESPACE TFW::
#     DESTINATION lib/cmake/TFW
# )
#
# # 生成配置文件
# # Generate configuration files
# include(CMakePackageConfigHelpers)
# write_basic_package_version_file(
#     TFWConfigVersion.cmake
#     VERSION ${PROJECT_VERSION}
#     COMPATIBILITY AnyNewerVersion
# )
#
# install(FILES
#     ${CMAKE_CURRENT_BINARY_DIR}/TFWConfigVersion.cmake
#     DESTINATION lib/cmake/TFW
# )
#
# # 安装可执行文件
# # Install executable files
# install(TARGETS TFW_cli
#     RUNTIME DESTINATION bin
# )

# ============================================================================
# build post-processing
# 构建后处理
# ============================================================================

# 显示构建信息
# Display build information
message(STATUS "TFW项目配置完成")
message(STATUS "构建目录: ${CMAKE_BINARY_DIR}")
message(STATUS "输出目录: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "库目录: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

# 如果生成了compile_commands.json，显示路径
# If compile_commands.json is generated, display the path
if(EXISTS "${CMAKE_BINARY_DIR}/compile_commands.json")
    message(STATUS "编译数据库: ${CMAKE_BINARY_DIR}/compile_commands.json")
    message(STATUS "IDE可以自动加载此文件进行智能提示")
    message(STATUS "IDE can automatically load this file for intelligent prompts")
endif()

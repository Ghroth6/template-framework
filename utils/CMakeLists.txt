# ============================================================================
# TFW Utils Module CMake Configuration
# TFW 工具模块 CMake 配置
# ============================================================================
#
# this file is used to configure the compilation of the TFW utils module
# 本文件用于配置 TFW 工具模块的编译
# it should conform to the TFW project coding standard requirements
# 它应符合 TFW 项目编码规范要求
#
# ============================================================================
#
# Compilation Instructions:
# 编译说明：
#
# 1. normal compilation: execute from the project root directory, it will automatically include this module
#    1. 标准编译：在项目根目录下执行，它将自动包含此模块
#    cd /path/to/template-framework
#    ./build_simple.sh
#
# 2. separate compilation (only needed for development and debugging):
#    2. 单独编译（主要用于开发和调试）：
#    cd utils
#    mkdir -p build && cd build
#    cmake .. -DCMAKE_BUILD_TYPE=Debug
#    make -j$(nproc)
#
# note: separate compilation is mainly used for development and debugging, please compile from the root directory for normal use
# 注意：单独编译主要用于开发和调试，正常使用时请从根目录进行编译
#
# ============================================================================

# set module name
# 设置模块名称
set(MODULE_NAME "TFW_utils")

# set module version
# 设置模块版本
set(MODULE_VERSION "1.0.0")

# set module description
# 设置模块描述
set(MODULE_DESCRIPTION "TFW framework utils module")

# ============================================================================
# source file configuration
# 源文件配置
# ============================================================================

# collect C source files
# 收集 C 源文件
set(UTILS_C_SOURCES
    log/src/TFW_log.c
    thread/src/TFW_thread.c
)

# 根据平台选择平台特定实现
# Select platform-specific implementations
if(WIN32)
    # Windows平台实现
    # Windows platform implementation
    list(APPEND UTILS_C_SOURCES
        timer/win32/TFW_timer_impl.c
        thread/win32/TFW_thread_impl.c
        file/win32/TFW_file_impl.c
        mem/win32/TFW_mem_impl.c)
elseif(APPLE)
    # macOS平台实现
    # macOS platform implementation
    list(APPEND UTILS_C_SOURCES
        timer/macos/TFW_timer_impl.c
        thread/posix/TFW_thread_impl.c
        file/posix/TFW_file_impl.c
        mem/posix/TFW_mem_impl.c)
else()
    # Linux/Unix平台实现（默认）
    # Linux/Unix platform implementation (default)
    list(APPEND UTILS_C_SOURCES
        timer/posix/TFW_timer_impl.c
        thread/posix/TFW_thread_impl.c
        file/posix/TFW_file_impl.c
        mem/posix/TFW_mem_impl.c)
endif()

# collect header files
# 收集头文件
set(UTILS_HEADERS
    include/TFW_common_defines.h
    include/TFW_log.h
    include/TFW_log_internal.h
    include/TFW_core_log.h
    include/TFW_utils_log.h
    include/TFW_timer.h
    include/TFW_thread.h
    include/TFW_file.h
    include/TFW_mem.h
    include/TFW_sdk_log.h
)

# ============================================================================
# create utils module library
# 创建工具模块库
# ============================================================================

# 根据全局设置决定库类型
# Determine library type based on global settings
if(BUILD_SHARED_LIBS)
    add_library(${MODULE_NAME} SHARED
        ${UTILS_C_SOURCES}
    )
    message(STATUS "Creating shared library for ${MODULE_NAME}")

    # 动态库特定属性设置
    # Shared library specific properties
    set_target_properties(${MODULE_NAME} PROPERTIES
        VERSION ${MODULE_VERSION}
        SOVERSION ${MODULE_VERSION}
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON
    )

    if(WIN32)
        set_target_properties(${MODULE_NAME} PROPERTIES
            WINDOWS_EXPORT_ALL_SYMBOLS ON
        )
    elseif(APPLE)
        # macOS平台动态库设置
        # macOS platform dynamic library setting
        set_target_properties(${MODULE_NAME} PROPERTIES
            MACOSX_BUNDLE FALSE
            INSTALL_NAME_DIR "@rpath"
        )
    else()
        # Linux/Unix平台动态库设置
        # Linux/Unix platform dynamic library setting
        set_target_properties(${MODULE_NAME} PROPERTIES
            INSTALL_RPATH_USE_LINK_PATH TRUE
            BUILD_WITH_INSTALL_RPATH TRUE
        )
    endif()
else()
    add_library(${MODULE_NAME} STATIC
        ${UTILS_C_SOURCES}
    )
    message(STATUS "Creating static library for ${MODULE_NAME}")
endif()

# set library properties
# 设置库属性
set_target_properties(${MODULE_NAME} PROPERTIES
    VERSION ${MODULE_VERSION}
    DESCRIPTION ${MODULE_DESCRIPTION}
    OUTPUT_NAME "TFW_utils"
)

# ============================================================================
# include directory configuration
# 包含目录配置
# ============================================================================

# set public header file directory (for other modules to use)
# 设置公共头文件目录（供其他模块使用）
target_include_directories(${MODULE_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# set private header file directory (only for this module to use)
# 设置私有头文件目录（仅供本模块使用）
target_include_directories(${MODULE_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/interface
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/log/include
    ${CMAKE_CURRENT_SOURCE_DIR}/timer/include
    ${CMAKE_CURRENT_SOURCE_DIR}/thread/include
    ${CMAKE_CURRENT_SOURCE_DIR}/file/include
    ${CMAKE_CURRENT_SOURCE_DIR}/mem/include
)

# ============================================================================
# compilation option configuration
# 编译选项配置
# ============================================================================

# set C standard
# 设置 C 标准
target_compile_features(${MODULE_NAME} PRIVATE
    c_std_11
)

# platform specific compilation options
# 平台特定编译选项
if(WIN32)
    # Windows platform specific options
    # Windows 平台特定选项
    target_compile_definitions(${MODULE_NAME} PRIVATE
        _WIN32_WINNT=0x0601
        WIN32_LEAN_AND_MEAN
        _CRT_SECURE_NO_WARNINGS
    )
elseif(APPLE)
    # macOS platform specific options
    # macOS 平台特定选项
    target_compile_definitions(${MODULE_NAME} PRIVATE
        __APPLE__
        _DARWIN_C_SOURCE
    )
else()
    # Linux/Unix platform specific options (default)
    # Linux/Unix 平台特定选项（默认）
    target_compile_definitions(${MODULE_NAME} PRIVATE
        __linux__
        _GNU_SOURCE
        _POSIX_C_SOURCE=200809L
    )
endif()

# ============================================================================
# link library configuration
# 链接库配置
# ============================================================================

# platform specific link library
# 平台特定链接库
if(WIN32)
    # Windows platform link library
    # Windows 平台链接库
    target_link_libraries(${MODULE_NAME} PRIVATE
        kernel32
        user32
        advapi32
    )
elseif(APPLE)
    # macOS platform link library
    # macOS 平台链接库
    target_link_libraries(${MODULE_NAME} PRIVATE
        pthread
        System
        stdc++
    )
else()
    # Linux/Unix platform link library (default)
    # Linux/Unix 平台链接库（默认）
    target_link_libraries(${MODULE_NAME} PRIVATE
        pthread
        rt
        dl
        stdc++
    )
endif()

# ============================================================================
# build information output
# 构建信息输出
# ============================================================================

message(STATUS "Configuration TFW utils module: ${MODULE_NAME}")
message(STATUS "Utils:module version: ${MODULE_VERSION}")
message(STATUS "Utils:module description: ${MODULE_DESCRIPTION}")
message(STATUS "Utils:module C source files: ${UTILS_C_SOURCES}")
message(STATUS "Utils:module header files: ${UTILS_HEADERS}")

message(STATUS "TFW utils module configuration completed")

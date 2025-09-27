# ============================================================================
# TFW Core Module CMake Configuration
# TFW 核心模块 CMake 配置
# ============================================================================
#
# this file is used to configure the compilation of the TFW core module
# 本文件用于配置 TFW 核心模块的编译
# it should conform to the TFW project coding standard requirements
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
#    cd core
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
set(MODULE_NAME "TFW_core")

# set module version
# 设置模块版本
set(MODULE_VERSION ${TFW_VERSION_STRING})

# set module description
# 设置模块描述
set(MODULE_DESCRIPTION "TFW framework core module")

# ============================================================================
# source file configuration
# ============================================================================

# collect C++ source files
# 收集 C++ 源文件
set(CORE_CPP_SOURCES
    TFW_core_interface.cpp
    src/TFW_core.cpp
    src/TFW_core_impl.cpp
    src/TFW_config_manager.cpp
    src/TFW_msg_loop_mgr.cpp
)

# collect C source files
# 收集 C 源文件
set(CORE_C_SOURCES
    TFW_core_interface.c
)

# collect header files
# 收集头文件
set(CORE_HEADERS
    include/TFW_types.h
    include/TFW_errorno.h
    include/TFW_single_instance.h
    include/TFW_core.h
    include/TFW_core_interface.h
    include/TFW_c_interface.h
    include/TFW_core_impl.h
    include/TFW_config_manager.h
)

# ============================================================================
# create core module library
# 创建核心模块库
# ============================================================================

# 根据全局设置决定库类型
# Determine library type based on global settings
if(BUILD_SHARED_LIBS)
    add_library(${MODULE_NAME} SHARED
        ${CORE_CPP_SOURCES}
        ${CORE_C_SOURCES}
    )
    message(STATUS "Creating shared library for ${MODULE_NAME}")
else()
    add_library(${MODULE_NAME} STATIC
        ${CORE_CPP_SOURCES}
        ${CORE_C_SOURCES}
    )
    message(STATUS "Creating static library for ${MODULE_NAME}")
endif()

# set library properties
# 设置库属性
set_target_properties(${MODULE_NAME} PROPERTIES
    VERSION ${MODULE_VERSION}
    DESCRIPTION ${MODULE_DESCRIPTION}
    OUTPUT_NAME "TFW_core"
)

# 动态库特定属性设置
# Shared library specific properties
if(BUILD_SHARED_LIBS)
    set_target_properties(${MODULE_NAME} PROPERTIES
        SOVERSION ${MODULE_VERSION}
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON
    )

    # 设置导出符号
    # Set export symbols
    if(WIN32)
        set_target_properties(${MODULE_NAME} PROPERTIES
            WINDOWS_EXPORT_ALL_SYMBOLS ON
        )
    elseif(APPLE)
        # macOS平台动态库设置
        set_target_properties(${MODULE_NAME} PROPERTIES
            MACOSX_BUNDLE FALSE
            INSTALL_NAME_DIR "@rpath"
        )
    else()
        # Linux/Unix平台动态库设置
        set_target_properties(${MODULE_NAME} PROPERTIES
            INSTALL_RPATH_USE_LINK_PATH TRUE
            BUILD_WITH_INSTALL_RPATH TRUE
        )
    endif()
endif()

# ============================================================================
# include directory configuration
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
    ${CMAKE_SOURCE_DIR}/core/include
)


# ============================================================================
# link library configuration
# ============================================================================

# 链接其他模块（如果存在）
# Link other modules if they exist
if(TARGET TFW_utils)
    target_link_libraries(${MODULE_NAME} PRIVATE TFW_utils)
    message(STATUS "Core module linked with utils module")
endif()

# platform specific link library
# 平台特定链接库
if(WIN32)
    # Windows platform link library
    # Windows 平台链接库
    target_link_libraries(${MODULE_NAME} PRIVATE
        kernel32
        user32
    )
elseif(APPLE)
    # macOS platform link library
    # macOS 平台链接库
    target_link_libraries(${MODULE_NAME} PRIVATE
        pthread
        stdc++
    )
else()
    # Linux/Unix platform link library (default)
    # Linux/Unix 平台链接库（默认）
    target_link_libraries(${MODULE_NAME} PRIVATE
        pthread
        rt
        stdc++
    )
endif()

# ============================================================================
# build information output
# 构建信息输出
# ============================================================================

message(STATUS "Configuration TFW core module: ${MODULE_NAME}")
message(STATUS "Core:module version: ${MODULE_VERSION}")
message(STATUS "Core:module description: ${MODULE_DESCRIPTION}")
message(STATUS "Core:module C++ source files: ${CORE_CPP_SOURCES}")
message(STATUS "Core:module C source files: ${CORE_C_SOURCES}")
message(STATUS "Core:module header files: ${CORE_HEADERS}")

message(STATUS "TFW core module configuration completed")

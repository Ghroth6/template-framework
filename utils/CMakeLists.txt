# Utils模块构建配置
cmake_minimum_required(VERSION 3.20)
project(TFW_utils)

# 构建选项
option(BUILD_UTILS_MODULAR "Build utils as separate modules" ON)

# 源文件
set(UTILS_SOURCES
    log/TFW_log.c
    log/TFW_log_file.c
    thread/TFW_thread.c
    config/TFW_config.c
)

# 根据平台选择平台特定实现
if(WIN32)
    # Windows平台实现
    list(APPEND UTILS_SOURCES
        thread/win32/TFW_thread_impl.c
        timer/win32/TFW_timer_impl.c
        file/win32/TFW_file_impl.c
        mem/win32/TFW_mem_impl.c
    )
elseif(APPLE)
    # macOS平台实现
    list(APPEND UTILS_SOURCES
        thread/posix/TFW_thread_impl.c
        timer/macos/TFW_timer_impl.c
        file/posix/TFW_file_impl.c
        mem/posix/TFW_mem_impl.c
    )
else()
    # Linux/Unix平台实现
    list(APPEND UTILS_SOURCES
        thread/posix/TFW_thread_impl.c
        timer/posix/TFW_timer_impl.c
        file/posix/TFW_file_impl.c
        mem/posix/TFW_mem_impl.c
    )
endif()

# 根据全局设置决定库类型
if(BUILD_SHARED_LIBS)
    add_library(TFW_utils SHARED ${UTILS_SOURCES})
    message(STATUS "Creating shared library for TFW_utils")

    # 动态库特定属性
    set_target_properties(TFW_utils PROPERTIES
        VERSION 1.0.0
        SOVERSION 1
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON
    )

    if(WIN32)
        set_target_properties(TFW_utils PROPERTIES
            WINDOWS_EXPORT_ALL_SYMBOLS ON
        )
    elseif(APPLE)
        # macOS平台动态库设置
        set_target_properties(TFW_utils PROPERTIES
            MACOSX_BUNDLE FALSE
            INSTALL_NAME_DIR "@rpath"
        )
    else()
        # Linux/Unix平台动态库设置
        set_target_properties(TFW_utils PROPERTIES
            INSTALL_RPATH_USE_LINK_PATH TRUE
            BUILD_WITH_INSTALL_RPATH TRUE
        )
    endif()
else()
    add_library(TFW_utils STATIC ${UTILS_SOURCES})
    message(STATUS "Creating static library for TFW_utils")
endif()

# 包含目录
target_include_directories(TFW_utils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../interface
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# 如果启用模块化构建，创建子模块库
if(BUILD_UTILS_MODULAR)
    # 日志模块
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/log/CMakeLists.txt")
        add_subdirectory(log)
    endif()

    # 线程模块
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/thread/CMakeLists.txt")
        add_subdirectory(thread)
    endif()

    # 配置模块
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/config/CMakeLists.txt")
        add_subdirectory(config)
    endif()

    message(STATUS "Utils modular build enabled")
endif()

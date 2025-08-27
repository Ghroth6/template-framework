# Utils模块构建配置
cmake_minimum_required(VERSION 3.20)
project(TFW_utils)

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

# 创建静态库
add_library(TFW_utils STATIC ${UTILS_SOURCES})

# 包含目录
target_include_directories(TFW_utils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../interface
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# Utils模块构建配置
cmake_minimum_required(VERSION 3.20)
project(TFW_utils)

# 源文件
set(UTILS_SOURCES
    log/TFW_log.c
)

# 创建静态库
add_library(TFW_utils STATIC ${UTILS_SOURCES})

# 包含目录
target_include_directories(TFW_utils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

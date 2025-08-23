# Core模块构建配置
project(TFW_core)

# 源文件
set(CORE_SOURCES
    TFW_core.cpp
    TFW_interface.cpp
    TFW_c_interface.cpp
)

# 创建静态库
add_library(TFW_core STATIC ${CORE_SOURCES})

# 包含目录
target_include_directories(TFW_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../interface
    ${CMAKE_CURRENT_SOURCE_DIR}/../utils/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../utils
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# 链接库
target_link_libraries(TFW_core
    TFW_utils
)

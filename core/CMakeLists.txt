# Core模块构建配置
cmake_minimum_required(VERSION 3.20)
project(TFW_core)

# 首先构建core_frame库（最底层）
add_library(TFW_core_frame STATIC
    core_frame/TFW_core.cpp
)

# 设置core_frame的包含目录
target_include_directories(TFW_core_frame PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../interface
    ${CMAKE_CURRENT_SOURCE_DIR}/../utils/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../utils
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# 链接core_frame的依赖库
target_link_libraries(TFW_core_frame
    TFW_utils
)

# 然后构建TFW_interface库（依赖core_frame）
add_library(TFW_interface_lib STATIC
    TFW_interface.cpp
)

# 设置TFW_interface的包含目录
target_include_directories(TFW_interface_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../interface
    ${CMAKE_CURRENT_SOURCE_DIR}/../utils/include
)

# 链接TFW_interface的依赖库
target_link_libraries(TFW_interface_lib
    TFW_core_frame
)

# 最后构建TFW_c_interface库（依赖TFW_interface和core_frame）
add_library(TFW_c_interface_lib STATIC
    TFW_c_interface.cpp
)

# 设置TFW_c_interface的包含目录
target_include_directories(TFW_c_interface_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../interface
    ${CMAKE_CURRENT_SOURCE_DIR}/../utils/include
)

# 链接TFW_c_interface的依赖库
target_link_libraries(TFW_c_interface_lib
    TFW_interface_lib
    TFW_core_frame
)

# 创建最终的TFW_core库，包含所有组件
add_library(TFW_core INTERFACE)

# 设置TFW_core的包含目录
target_include_directories(TFW_core INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../interface
    ${CMAKE_CURRENT_SOURCE_DIR}/../utils/include
)

# 链接所有子库
target_link_libraries(TFW_core INTERFACE
    TFW_core_frame
    TFW_interface_lib
    TFW_c_interface_lib
)

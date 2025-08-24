# C++版本示例构建配置
cmake_minimum_required(VERSION 3.20)
project(TFW_Simple_CPP)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../out/cpp)

# 根据平台设置库文件扩展名和前缀
if(WIN32)
    set(LIB_EXT ".lib")
    set(LIB_PREFIX "")
    set(BUILD_CONFIG_DIR "Release")
else()
    set(LIB_EXT ".a")
    set(LIB_PREFIX "lib")
    set(BUILD_CONFIG_DIR "")
endif()

# 源文件
set(CPP_SOURCES
    main.cpp
)

# 创建可执行文件
add_executable(tfw_simple_cpp ${CPP_SOURCES})

# 包含目录
target_include_directories(tfw_simple_cpp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../interface
    ${CMAKE_CURRENT_SOURCE_DIR}/../../core/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../utils/include
)

# 链接库 - 使用平台相关的路径和扩展名
target_link_libraries(tfw_simple_cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../build/core_build/${BUILD_CONFIG_DIR}/${LIB_PREFIX}TFW_interface_lib${LIB_EXT}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../build/core_build/${BUILD_CONFIG_DIR}/${LIB_PREFIX}TFW_core_frame${LIB_EXT}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../build/utils_build/${BUILD_CONFIG_DIR}/${LIB_PREFIX}TFW_utils${LIB_EXT}
)

# 设置输出名称
set_target_properties(tfw_simple_cpp PROPERTIES
    OUTPUT_NAME "tfw_simple_cpp"
)

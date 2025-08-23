# Simple 示例项目根构建配置
cmake_minimum_required(VERSION 3.16)
project(TFW_Simple_Examples)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../out)

# 添加子项目
add_subdirectory(c)
add_subdirectory(cpp)

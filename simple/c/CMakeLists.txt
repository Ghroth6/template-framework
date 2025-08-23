# C版本示例构建配置
project(TFW_Simple_C)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../out/c)

# 源文件
set(C_SOURCES
    main.c
)

# 创建可执行文件
add_executable(tfw_simple_c ${C_SOURCES})

# 包含目录
target_include_directories(tfw_simple_c PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../interface
    ${CMAKE_CURRENT_SOURCE_DIR}/../../core
    ${CMAKE_CURRENT_SOURCE_DIR}/../../utils/include
)

# 链接库
target_link_libraries(tfw_simple_c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../core/libTFW_core.a
    ${CMAKE_CURRENT_SOURCE_DIR}/../../utils/libTFW_utils.a
)

# 设置输出名称
set_target_properties(tfw_simple_c PROPERTIES
    OUTPUT_NAME "tfw_simple_c"
)

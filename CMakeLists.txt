cmake_minimum_required(VERSION 3.20)
project(TempleFrameWork)

# 璁剧疆C++鏍囧噯
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 缁熶竴杈撳嚭鍒皁ut鐩綍
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out/lib)

# 鍖呭惈鐩綍
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/interface
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/include
)

# 娣诲姞瀛愮洰褰?add_subdirectory(utils)
add_subdirectory(core)


# 娣诲姞simple绀轰緥椤圭洰锛堝彲閫夛級
# add_subdirectory(simple)

# Core妯″潡鏋勫缓閰嶇疆
project(TFW_core)

# 婧愭枃浠?
set(CORE_SOURCES
    TFW_core.cpp
    TFW_interface.cpp
    TFW_c_interface.cpp
)

# 鍒涘缓闈欐€佸簱
add_library(TFW_core STATIC ${CORE_SOURCES})

# 鍖呭惈鐩綍
target_include_directories(TFW_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../interface
    ${CMAKE_CURRENT_SOURCE_DIR}/../utils/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../utils
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# 閾炬帴搴?
target_link_libraries(TFW_core
    TFW_utils
)

# Utils妯″潡鏋勫缓閰嶇疆
project(TFW_utils)

# 婧愭枃浠?set(UTILS_SOURCES
    log/TFW_log.c
)

# 鍒涘缓闈欐€佸簱
add_library(TFW_utils STATIC ${UTILS_SOURCES})

# 鍖呭惈鐩綍
target_include_directories(TFW_utils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

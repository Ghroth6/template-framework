# C鐗堟湰绀轰緥鏋勫缓閰嶇疆
project(TFW_Simple_C)

# 璁剧疆杈撳嚭鐩綍
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../out/c)

# 婧愭枃浠?
set(C_SOURCES
    main.c
)

# 鍒涘缓鍙墽琛屾枃浠?
add_executable(tfw_simple_c ${C_SOURCES})

# 鍖呭惈鐩綍
target_include_directories(tfw_simple_c PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../interface
    ${CMAKE_CURRENT_SOURCE_DIR}/../../core
    ${CMAKE_CURRENT_SOURCE_DIR}/../../utils/include
)

# 閾炬帴搴?
target_link_libraries(tfw_simple_c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../core/libTFW_core.a
    ${CMAKE_CURRENT_SOURCE_DIR}/../../utils/libTFW_utils.a
)

# 璁剧疆杈撳嚭鍚嶇О
set_target_properties(tfw_simple_c PROPERTIES
    OUTPUT_NAME "tfw_simple_c"
)

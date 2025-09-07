# ============================================================================
# TFW Third Party Libraries Management
# TFW 第三方库管理
# ============================================================================

# 保存当前BUILD_SHARED_LIBS设置
set(TFW_BUILD_SHARED_LIBS_OLD ${BUILD_SHARED_LIBS})

# 检查并添加cjson库
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cjson/CMakeLists.txt")
    # 根据项目BUILD_SHARED_LIBS设置决定cjson构建方式
    # 项目规范：开发时使用静态库，发布时使用动态库
    if(BUILD_SHARED_LIBS)
        set(BUILD_SHARED_LIBS ON)
        message(STATUS "Building cjson as shared library")
    else()
        set(BUILD_SHARED_LIBS OFF)
        message(STATUS "Building cjson as static library")
    endif()

    # 设置cjson不构建测试和工具
    set(ENABLE_CJSON_TEST OFF CACHE BOOL "Disable cjson tests")
    set(ENABLE_CJSON_UTILS OFF CACHE BOOL "Disable cjson utils")
    set(ENABLE_CJSON_UNINSTALL OFF CACHE BOOL "Disable cjson uninstall target")

    # 添加cjson子目录
    add_subdirectory(cjson)

    # 创建导入库目标别名，便于统一引用
    if(TARGET cjson)
        add_library(thirdparty::cjson ALIAS cjson)
        # 设置cjson的PUBLIC包含目录，使依赖它的模块能自动找到头文件
        # 使用相对路径避免CMake的源目录前缀限制
        target_include_directories(cjson PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/cjson>
            $<INSTALL_INTERFACE:include/cjson>
        )
        message(STATUS "Third party library cjson added as target thirdparty::cjson")
    endif()
else()
    message(WARNING "cjson library CMakeLists.txt not found")
endif()

# 可以在这里添加更多第三方库
# Example for adding more third party libraries:
# if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/other_lib/CMakeLists.txt")
#     add_subdirectory(other_lib)
# endif()

# 恢复原来的BUILD_SHARED_LIBS设置
set(BUILD_SHARED_LIBS ${TFW_BUILD_SHARED_LIBS_OLD})

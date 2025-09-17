# ============================================================================
# TFW CLI Module Configuration
# TFW CLI 模块配置
# ============================================================================

# Module information
# 模块信息
set(MODULE_NAME "TFW_cli")
set(MODULE_VERSION "1.0.0")
set(MODULE_DESCRIPTION "TFW Command Line Interface Tool")

# ============================================================================
# CLI source files configuration
# CLI 源文件配置
# ============================================================================

# CLI tool source files
# CLI 工具源文件
set(CLI_SOURCES
    TFW_cli.c
    TFW_cli_config.c
    # If you need other source files, add them here
    # 如果您需要其他源文件，请在此处添加
    # other_file.c
)

# CLI tool header files
# CLI 工具头文件
set(CLI_HEADERS
    # Add header files here if needed
    # 如果需要，请在此处添加头文件
)

# ============================================================================
# CLI executable target configuration
# CLI 可执行文件目标配置
# ============================================================================

# Create CLI executable
# 创建 CLI 可执行文件
add_executable(${MODULE_NAME} ${CLI_SOURCES})

# Set target properties
# 设置目标属性
set_target_properties(${MODULE_NAME} PROPERTIES
    VERSION ${MODULE_VERSION}
    DESCRIPTION ${MODULE_DESCRIPTION}
    OUTPUT_NAME ${MODULE_NAME}
)

# ============================================================================
# Include directories configuration
# 包含目录配置
# ============================================================================

# Public include directories (for other modules that might depend on CLI)
# 公共包含目录（供可能依赖 CLI 的其他模块使用）
target_include_directories(${MODULE_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/interface
)

# Private include directories (for CLI internal use)
# 私有包含目录（供 CLI 内部使用）
target_include_directories(${MODULE_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/utils/include
    ${CMAKE_SOURCE_DIR}/core/include
    ${CMAKE_SOURCE_DIR}/cli/include
)

# ============================================================================
# Dependencies and linking configuration
# 依赖和链接配置
# ============================================================================

# Link with utils module (if available)
# 链接 utils 模块（如果可用）
if(TARGET TFW_utils)
    target_link_libraries(${MODULE_NAME} PRIVATE TFW_utils)
    message(STATUS "CLI tool linked with utils module")
else()
    message(WARNING "Utils module not available, CLI tool will be built without utils support")
endif()

# Link with core module (if available)
# 链接 core 模块（如果可用）
if(TARGET TFW_core)
    target_link_libraries(${MODULE_NAME} PRIVATE TFW_core)
    message(STATUS "CLI tool linked with core module")
else()
    message(STATUS "Core module not available, CLI tool will be built without core support")
endif()

# ============================================================================
# Platform-specific configuration
# 平台特定配置
# ============================================================================

# Windows platform specific settings
# Windows 平台特定设置
if(WIN32)
    # Windows specific compile definitions
    # Windows 特定编译定义
    target_compile_definitions(${MODULE_NAME} PRIVATE
        _CRT_SECURE_NO_WARNINGS
        WIN32_LEAN_AND_MEAN
    )

    # Windows specific link libraries
    # Windows 特定链接库
    target_link_libraries(${MODULE_NAME} PRIVATE
        advapi32
        shell32
    )
endif()

# macOS platform specific settings
# macOS 平台特定设置
if(APPLE)
    # macOS specific compile definitions
    # macOS 特定编译定义
    target_compile_definitions(${MODULE_NAME} PRIVATE
        _DARWIN_C_SOURCE
    )

    # macOS specific link libraries
    # macOS 特定链接库
    target_link_libraries(${MODULE_NAME} PRIVATE
        System
    )
endif()

# Linux/Unix platform specific settings
# Linux/Unix 平台特定设置
if(UNIX AND NOT APPLE)
    # Linux specific compile definitions
    # Linux 特定编译定义
    target_compile_definitions(${MODULE_NAME} PRIVATE
        _POSIX_C_SOURCE=200809L
    )

    # Linux specific link libraries
    # Linux 特定链接库
    target_link_libraries(${MODULE_NAME} PRIVATE
        dl
        pthread
    )
endif()

# ============================================================================
# Installation configuration
# 安装配置
# ============================================================================

# Install CLI executable
# 安装 CLI 可执行文件
install(TARGETS ${MODULE_NAME}
    RUNTIME DESTINATION bin
    COMPONENT cli
)

# Install CLI documentation (if exists)
# 安装 CLI 文档（如果存在）
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/README.md)
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/README.md
        DESTINATION share/doc/${MODULE_NAME}
        COMPONENT cli
    )
endif()

# ============================================================================
# Build information display
# 构建信息显示
# ============================================================================

message(STATUS "CLI Module Configuration:")
message(STATUS "  Module Name: ${MODULE_NAME}")
message(STATUS "  Version: ${MODULE_VERSION}")
message(STATUS "  Description: ${MODULE_DESCRIPTION}")
message(STATUS "  Source Files: ${CLI_SOURCES}")
message(STATUS "  Output Name: ${MODULE_NAME}")

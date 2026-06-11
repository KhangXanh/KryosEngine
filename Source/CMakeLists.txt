cmake_minimum_required(VERSION 3.20)
project(KryosEngine)

# =============================================================================
# C++23
# =============================================================================
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# =============================================================================
# Backend options
# =============================================================================
option(KRYOS_BACKEND_VULKAN "Build with Vulkan backend" ON)

if(NOT KRYOS_BACKEND_VULKAN)
    message(FATAL_ERROR "KRYOS_BACKEND_VULKAN must be enabled.")
endif()

# =============================================================================
# ThirdParty paths
# =============================================================================
set(THIRDPARTY_DIR "${CMAKE_SOURCE_DIR}/ThirdParty")

set(GLFW_DIR     "${THIRDPARTY_DIR}/glfw")
set(YAML_CPP_DIR "${THIRDPARTY_DIR}/yaml-cpp")
set(NFD_DIR      "${THIRDPARTY_DIR}/nfd")
set(STB_DIR      "${THIRDPARTY_DIR}/stb_image")

# =============================================================================
# GLFW  (submodule)
# =============================================================================
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)

add_subdirectory(${GLFW_DIR} ${CMAKE_BINARY_DIR}/ThirdParty/glfw)

# =============================================================================
# yaml-cpp  (submodule)
# =============================================================================
set(YAML_CPP_BUILD_TESTS   OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_TOOLS   OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "" FORCE)
set(YAML_CPP_INSTALL       OFF CACHE BOOL "" FORCE)
set(YAML_CPP_FORMAT_SOURCE OFF CACHE BOOL "" FORCE)

add_subdirectory(${YAML_CPP_DIR} ${CMAKE_BINARY_DIR}/ThirdParty/yaml-cpp)

# =============================================================================
# NFD  (plain source folder)
# =============================================================================
if(WIN32)
    set(NFD_IMPL "${NFD_DIR}/src/nfd_win.cpp")
elseif(APPLE)
    set(NFD_IMPL "${NFD_DIR}/src/nfd_cocoa.m")
else()
    set(NFD_IMPL "${NFD_DIR}/src/nfd_gtk.c")
endif()

if(NOT EXISTS "${NFD_IMPL}")
    message(FATAL_ERROR "NFD implementation not found at: ${NFD_IMPL}")
endif()

add_library(nfd STATIC ${NFD_IMPL})
target_include_directories(nfd PUBLIC "${NFD_DIR}/src/include" "${NFD_DIR}/include")

if(WIN32)
    target_link_libraries(nfd PUBLIC ole32 uuid shell32 comdlg32)
endif()

# =============================================================================
# stb_image  (plain source folder)
# =============================================================================
add_library(stb_image STATIC "${STB_DIR}/stb_image.cpp")
target_include_directories(stb_image PUBLIC "${STB_DIR}")

# =============================================================================
# Vulkan
# =============================================================================
find_package(Vulkan REQUIRED)

# =============================================================================
# Engine sources
# =============================================================================
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
    "Engine/*.cpp"
    "Engine/*.hpp"
    "Engine/*.h"
)

# =============================================================================
# KryosEngine  (shared library / DLL)
# =============================================================================
add_library(KryosEngine SHARED ${ENGINE_SOURCES})

target_compile_definitions(KryosEngine
    PRIVATE
        KRYOS_ENGINE_EXPORT
        KryosEngine_EXPORTS
    PUBLIC
        KS_DEBUG_MODE           # Comment out for release
        YAML_CPP_STATIC_DEFINE
        KRYOS_HAS_VULKAN
)

target_include_directories(KryosEngine
    PUBLIC
        "${CMAKE_SOURCE_DIR}/Engine"
        "${CMAKE_SOURCE_DIR}"
)

target_link_libraries(KryosEngine
    PUBLIC
        glfw
        yaml-cpp
        Vulkan::Vulkan
    PRIVATE
        stb_image
        nfd
)

if(WIN32)
    target_link_libraries(KryosEngine PRIVATE
        user32 gdi32 shell32 dwmapi comdlg32 ole32 uuid
    )
    if(MINGW)
        target_link_libraries(KryosEngine PRIVATE stdc++exp)
    endif()
endif()

# =============================================================================
# KryosStudio  (executable)
# =============================================================================
file(GLOB_RECURSE APPLICATION_SOURCES CONFIGURE_DEPENDS
    "Applications/*.cpp"
    "Applications/*.hpp"
    "Applications/*.h"
)

add_executable(KryosStudio ${APPLICATION_SOURCES})

target_include_directories(KryosStudio PRIVATE
    "${CMAKE_SOURCE_DIR}/Applications"
    "${CMAKE_SOURCE_DIR}/Engine"
    "${CMAKE_SOURCE_DIR}"
)

target_link_libraries(KryosStudio PRIVATE KryosEngine)

# Keep DLL and EXE in the same output directory
set_target_properties(KryosEngine KryosStudio PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

# =============================================================================
# Build summary
# =============================================================================
message(STATUS "=== Kryos Engine Build Config ===")
message(STATUS "  Vulkan backend : ${KRYOS_BACKEND_VULKAN}")
message(STATUS "  Build type     : ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ standard   : ${CMAKE_CXX_STANDARD}")
message(STATUS "=================================")
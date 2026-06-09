# Copyright (c) 2026 Nguyễn Mạnh Khang (Blue). All Rights Reserved.
# Distributed under the MIT License. (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.20)
project(KryosEngine)

# C++23
# -------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# -------------------------------------------------------------------

# Backend selection
# -------------------------------------------------------------------
option(KRYOS_BACKEND_OPENGL "Build with OpenGL backend" ON)
option(KRYOS_BACKEND_VULKAN "Build with Vulkan backend" ON)

if(NOT KRYOS_BACKEND_OPENGL AND NOT KRYOS_BACKEND_VULKAN)
    message(FATAL_ERROR "At least one backend (KRYOS_BACKEND_OPENGL or KRYOS_BACKEND_VULKAN) must be enabled.")
endif()
# -------------------------------------------------------------------

# ThirdParty paths
# -------------------------------------------------------------------
set(GLFW_DIR "${CMAKE_SOURCE_DIR}/ThirdParty/glfw")
set(GLM_DIR "${CMAKE_SOURCE_DIR}/ThirdParty/glm")
set(GLAD_DIR "${CMAKE_SOURCE_DIR}/ThirdParty/glad")
set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/ThirdParty/imgui")
set(NFD_DIR "${CMAKE_SOURCE_DIR}/ThirdParty/nfd")
set(YAML_CPP_DIR "${CMAKE_SOURCE_DIR}/ThirdParty/yaml-cpp")
# -------------------------------------------------------------------

# GLFW
# -------------------------------------------------------------------
if(WIN32)
    set(GLFW_LIB_PATH "${GLFW_DIR}/lib-static-ucrt/libglfw3.a")
elseif(APPLE)
    set(GLFW_LIB_PATH "${GLFW_DIR}/lib-macos/libglfw3.a")
else()
    find_package(glfw3 QUIET)
    if(NOT glfw3_FOUND)
        set(GLFW_LIB_PATH "${GLFW_DIR}/lib-linux/libglfw3.a")
    endif()
endif()
# -------------------------------------------------------------------

# NFD
# -------------------------------------------------------------------
if(WIN32)
    set(NFD_SOURCES "${NFD_DIR}/src/nfd_win.cpp")
elseif(APPLE)
    set(NFD_SOURCES "${NFD_DIR}/src/nfd_cocoa.m")
else()
    set(NFD_SOURCES "${NFD_DIR}/src/nfd_gtk.c")
endif()

if(EXISTS "${NFD_SOURCES}")
    add_library(nfd STATIC ${NFD_SOURCES})
    target_include_directories(nfd PUBLIC "${NFD_DIR}/src/include")
    if(WIN32)
        target_link_libraries(nfd PUBLIC ole32 uuid shell32 comdlg32)
    endif()
else()
    message(WARNING "Can't find the NFD's implementation file at: ${NFD_SOURCES}")
endif()
# -------------------------------------------------------------------

# STB_IMAGE
# -------------------------------------------------------------------
add_library(stb_image STATIC ThirdParty/stb_image/stb_image.cpp)
target_include_directories(stb_image PUBLIC ThirdParty/stb_image)
# -------------------------------------------------------------------

# YAML-CPP
# -------------------------------------------------------------------
file(GLOB_RECURSE YAML_CPP_SOURCES "${YAML_CPP_DIR}/src/*.cpp")
add_library(yaml-cpp STATIC ${YAML_CPP_SOURCES})
target_include_directories(yaml-cpp PUBLIC "${YAML_CPP_DIR}/include")
target_compile_definitions(yaml-cpp PUBLIC YAML_CPP_STATIC_DEFINE)
# -------------------------------------------------------------------

# OPENGL define
# -------------------------------------------------------------------
if(KRYOS_BACKEND_OPENGL)
    # GLAD
    add_library(glad STATIC ${GLAD_DIR}/src/glad.c)
    target_include_directories(glad PUBLIC ${GLAD_DIR}/include)

    # ImGui (OpenGL)
    add_library(imgui_opengl STATIC
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    )
    target_include_directories(imgui_opengl PUBLIC
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
        ${GLFW_DIR}/include
    )
    target_link_libraries(imgui_opengl PUBLIC glad)
    target_compile_definitions(imgui_opengl PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)
endif()
# -------------------------------------------------------------------

# VULKAN define
# -------------------------------------------------------------------
if(KRYOS_BACKEND_VULKAN)
    find_package(Vulkan REQUIRED)

    # ImGui (Vulkan)
    add_library(imgui_vulkan STATIC
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
    )
    target_include_directories(imgui_vulkan PUBLIC
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
        ${GLFW_DIR}/include
        ${Vulkan_INCLUDE_DIRS}
    )
    target_link_libraries(imgui_vulkan PUBLIC Vulkan::Vulkan)
endif()
# -------------------------------------------------------------------

# Sources
# -------------------------------------------------------------------
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
    "Engine/*.cpp"
    "Engine/*.hpp"
    "Engine/*.h"
    "Engine/Core/Core.hpp"
)

file(GLOB_RECURSE APPLICATION_SOURCES CONFIGURE_DEPENDS
    "Applications/*.cpp"
    "Applications/*.hpp"
    "Applications/*.h"
)
# -------------------------------------------------------------------

# Compile to KryosEngine.dll (lib file)
# -------------------------------------------------------------------
add_library(KryosEngine SHARED ${ENGINE_SOURCES})

target_compile_definitions(KryosEngine PRIVATE KRYOS_ENGINE_EXPORT)

# We switch between debug and release (Comment to turn off)
target_compile_definitions(KryosEngine PUBLIC KS_DEBUG_MODE)

target_include_directories(KryosEngine PUBLIC
    "${CMAKE_SOURCE_DIR}/Engine"
    "${CMAKE_SOURCE_DIR}"
    ${GLFW_DIR}/include
    ${GLM_DIR}
    ${NFD_DIR}/include
    "${YAML_CPP_DIR}/include"
)
# -------------------------------------------------------------------


# Link OpenGL and Vulkan
# -------------------------------------------------------------------
if(KRYOS_BACKEND_OPENGL)
    target_compile_definitions(KryosEngine PUBLIC KRYOS_HAS_OPENGL)
    target_link_libraries(KryosEngine PRIVATE imgui_opengl glad)
endif()

if(KRYOS_BACKEND_VULKAN)
    target_compile_definitions(KryosEngine PUBLIC KRYOS_HAS_VULKAN)
    target_link_libraries(KryosEngine PRIVATE imgui_vulkan Vulkan::Vulkan)
endif()
# -------------------------------------------------------------------

# Link ThirdParty to DLL
target_link_libraries(KryosEngine PRIVATE stb_image nfd yaml-cpp)

# Link GLFW to DLL
if(TARGET glfw)
    target_link_libraries(KryosEngine PRIVATE glfw)
else()
    target_link_libraries(KryosEngine PRIVATE "${GLFW_LIB_PATH}")
endif()

# System Libraries for Windows
# -------------------------------------------------------------------
if(WIN32)
    target_link_libraries(KryosEngine PRIVATE user32 gdi32 shell32 dwmapi comdlg32 ole32 uuid)
    if(MINGW)
        target_link_libraries(KryosEngine PRIVATE stdc++exp)
    endif()
    if(KRYOS_BACKEND_OPENGL)
        target_link_libraries(KryosEngine PRIVATE opengl32)
    endif()
endif()
# -------------------------------------------------------------------

# The main executable
# -------------------------------------------------------------------
add_executable(KryosStudio ${APPLICATION_SOURCES})

target_include_directories(KryosStudio PRIVATE
    "${CMAKE_SOURCE_DIR}/Applications"
    "${CMAKE_SOURCE_DIR}/Engine"
    "${CMAKE_SOURCE_DIR}"
    ${GLM_DIR}
)

target_link_libraries(KryosStudio PRIVATE KryosEngine)
# -------------------------------------------------------------------

# Logging
# -------------------------------------------------------------------
message(STATUS "=== Kryos Engine Build Config ===")
message(STATUS "  OpenGL backend : ${KRYOS_BACKEND_OPENGL}")
message(STATUS "  Vulkan backend : ${KRYOS_BACKEND_VULKAN}")
message(STATUS "=================================")
# -------------------------------------------------------------------
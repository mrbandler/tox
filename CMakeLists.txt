cmake_minimum_required(VERSION 3.21)

project(tox
    VERSION 0.1.0
    DESCRIPTION "Tox - A typed Lox implementation"
    LANGUAGES CXX
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for LSP support (clangd)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Compiler warnings
if(MSVC)
    add_compile_options(/W4 /WX /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Options
option(TOX_BUILD_SHARED "Build shared library" ON)
option(TOX_BUILD_STATIC "Build static library" ON)
option(TOX_BUILD_BINARY "Build standalone executable" ON)

# Third-party dependencies
include(FetchContent)

# magic_enum - Enum reflection (used by library)
FetchContent_Declare(
    magic_enum
    GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
    GIT_TAG        v0.9.6
)

FetchContent_MakeAvailable(magic_enum)

# CLI11 - Command line argument parser (binary only)
if(TOX_BUILD_BINARY)
    FetchContent_Declare(
        cli11
        GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
        GIT_TAG        v2.4.2
    )

    FetchContent_MakeAvailable(cli11)
endif()

# Library source files (auto-detect all .cpp files except main.cpp)
file(GLOB_RECURSE TOX_LIB_SOURCES
    CONFIGURE_DEPENDS
    "src/*.cpp"
)
list(FILTER TOX_LIB_SOURCES EXCLUDE REGEX ".*main\\.cpp$")

# Public headers (auto-detect)
file(GLOB_RECURSE TOX_PUBLIC_HEADERS
    CONFIGURE_DEPENDS
    "include/*.h" "include/*.hpp"
)

# Shared library
if(TOX_BUILD_SHARED)
    add_library(tox_shared SHARED ${TOX_LIB_SOURCES})
    set_target_properties(tox_shared PROPERTIES
        OUTPUT_NAME tox
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        PUBLIC_HEADER "${TOX_PUBLIC_HEADERS}"
    )
    target_include_directories(tox_shared
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
    target_link_libraries(tox_shared PUBLIC magic_enum::magic_enum)

    # Export symbols on Windows
    if(WIN32)
        target_compile_definitions(tox_shared PRIVATE TOX_BUILD_DLL)
    endif()
endif()

# Static library
if(TOX_BUILD_STATIC)
    add_library(tox_static STATIC ${TOX_LIB_SOURCES})
    set_target_properties(tox_static PROPERTIES
        OUTPUT_NAME tox_static
        VERSION ${PROJECT_VERSION}
        PUBLIC_HEADER "${TOX_PUBLIC_HEADERS}"
    )
    target_include_directories(tox_static
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
    target_link_libraries(tox_static PUBLIC magic_enum::magic_enum)
endif()

# Main executable
if(TOX_BUILD_BINARY)
    add_executable(tox src/main.cpp)

    # Link against static library by default
    if(TOX_BUILD_STATIC)
        target_link_libraries(tox PRIVATE tox_static CLI11::CLI11)
    elseif(TOX_BUILD_SHARED)
        target_link_libraries(tox PRIVATE tox_shared CLI11::CLI11)
    else()
        # Fallback: compile sources directly into executable
        target_sources(tox PRIVATE ${TOX_LIB_SOURCES})
        target_include_directories(tox PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${CMAKE_CURRENT_SOURCE_DIR}/src
        )
        target_link_libraries(tox PRIVATE magic_enum::magic_enum CLI11::CLI11)
    endif()
endif()

# Installation
include(GNUInstallDirs)

if(TOX_BUILD_SHARED)
    install(TARGETS tox_shared
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
endif()

if(TOX_BUILD_STATIC)
    install(TARGETS tox_static
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
endif()

if(TOX_BUILD_BINARY)
    install(TARGETS tox
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif()

# Install headers
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

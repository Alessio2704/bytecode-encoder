cmake_minimum_required(VERSION 4.0)
project(BytecodeEncoder VERSION 1.0)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options("-Wall" "-Wextra" "-Wpedantic" "-Wconversion" "-Wsign-conversion")
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        add_compile_options("-Wdocumentation")
    endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options("/W4" "/WX")
endif()

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(BytecodeEncoder INTERFACE)

target_include_directories(BytecodeEncoder INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG "v1.14.0"
)
FetchContent_MakeAvailable(googletest)

add_subdirectory(tests)
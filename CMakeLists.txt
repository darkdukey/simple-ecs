cmake_minimum_required(VERSION 2.8.12)

# Project name
project(coco-core)

# Build type
# set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_CXX_FLAGS_RELEASE "/MT")
# set(CMAKE_CXX_FLAGS_DEBUG "/MTd")

# include build scripts from dependenceis
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
conan_basic_setup()

# Create library
include_directories("include")
file(GLOB HEADER_FILES "src/*.h" )
file(GLOB SRC_FILES "src/*.cpp" )

add_library(coco-core ${SRC_FILES} ${HEADER_FILES})
target_compile_features(coco-core PRIVATE cxx_std_17)
target_include_directories(coco-core
    PUBLIC 
        $<INSTALL_INTERFACE:include>   
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

# Set warnings
function(setWarnings target)
    if(MSVC)
    else()
        target_compile_options(${target} PRIVATE
            -Wall
            -Wextra
            -Wshadow
            -Wnon-virtual-dtor
            -Wold-style-cast
            -Wcast-align
            -Wunused
            -Woverloaded-virtual
            -Wpedantic
            -Wconversion
            -Wsign-conversion
            -Wmisleading-indentation
            -Wduplicated-cond
            -Wduplicated-branches
            -Wlogical-op
            -Wnull-dereference
            -Wuseless-cast
            -Wdouble-promotion
            -Wno-unknown-pragmas
            -Wno-sign-conversion
            )
        endif()
endfunction()

# Set standard
function(setStandard target)
    target_compile_features(${target} PRIVATE cxx_std_17)
endfunction()

# Code coverage
option(CODE_COVERAGE "Enable coverage reporting" OFF)
if(CODE_COVERAGE)
    target_compile_options(coco-core INTERFACE
        -O0
        -g
        --coverage)
    target_link_libraries(coco-core INTERFACE --coverage)
endif()

# Build Sample
option (BUILD_SAMPLES "Build the samples" ON)
if (BUILD_SAMPLES)
    add_subdirectory(sample)
endif()

# Build Tests
option (BUILD_TESTING "Build the testing" ON)
if (BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

# Benchmarks
option (BUILD_BENCHMARKS "Build the benchmarks." ON)
if (BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()
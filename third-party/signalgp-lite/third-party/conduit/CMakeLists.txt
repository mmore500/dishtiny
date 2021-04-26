cmake_minimum_required(VERSION 3.13...3.18)

project(conduit VERSION 0.1.0
        DESCRIPTION ""
        LANGUAGES CXX
)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

# required libraries
find_package(benchmark REQUIRED)
find_package(Threads REQUIRED)
set(MPI_DETERMINE_LIBRARY_VERSION TRUE)
find_package(MPI REQUIRED)
find_package(ZLIB REQUIRED)
find_package(CURL REQUIRED)
find_package(METIS)

add_definitions(-DOMPI_SKIP_MPICXX)
#include_directories(SYSTEM ${MPI_C_INCLUDE_PATH})

include(CTest)
enable_testing()


set(CMAKE_CONFIGURATION_TYPES "Debug;Opt")
set(CMAKE_CXX_FLAGS_DEBUG "-D_GLIBCXX_DEBUG -D_LIBCPP_DEBUG -DUITSL_AUDIT_OCCUPANCY -pipe -Wall -Wno-unused-function")
set(CMAKE_CXX_FLAGS_OPT "-pipe -pthread -march=native -DNDEBUG -O3 -flto -Wno-unused-function -Wno-unused-value")
set(CMAKE_CXX_FLAGS_COVERAGE "-std=c++17 -pipe -pthread -march=native -g -Wall -Wno-unused-function -I$(TO_ROOT)/coverage_include/ -I$(TO_ROOT)/third-party/ -DUITSL_AUDIT_OCCUPANCY -Wnon-virtual-dtor -Wcast-align -Woverloaded-virtual -ftemplate-backtrace-limit=0 -fprofile-instr-generate -fcoverage-mapping -fno-inline -fno-elide-constructors -O0")
#set(CMAKE_CXX_FLAGS "-pipe -Wall -Wno-unused-function -Wno-unused-lambda-capture")
message("Generated with config types: ${CMAKE_CONFIGURATION_TYPES}")
set(default_build_type "Debug")

message("Building in ${CMAKE_BUILD_TYPE} mode.")

set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CATCH_BUILD_TESTING ON)
set(BUILD_TESTING ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." TRUE)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
   add_compile_options (-fdiagnostics-color=always)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
   add_compile_options (-fcolor-diagnostics)
endif ()

# create conduit library
add_library(conduit INTERFACE)
# target_sources(conduit INTERFACE /include)
# add include dirs
target_include_directories(conduit INTERFACE include/)
target_include_directories(conduit INTERFACE third-party/)

target_compile_features(conduit INTERFACE cxx_std_17)

# add link flags
target_link_libraries(conduit INTERFACE stdc++fs)
target_link_libraries(conduit INTERFACE Threads::Threads)
target_link_libraries(conduit INTERFACE benchmark::benchmark)
target_link_libraries(conduit INTERFACE MPI::MPI_CXX)
target_link_libraries(conduit INTERFACE ZLIB::ZLIB)
target_link_libraries(conduit INTERFACE CURL::libcurl)
target_link_libraries(conduit INTERFACE metis)

# enable interprocedural optimization (-flto flag)
include(CheckIPOSupported)
check_ipo_supported(RESULT result)
if(result)
#  set_target_properties(conduit PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

add_subdirectory(tests)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)

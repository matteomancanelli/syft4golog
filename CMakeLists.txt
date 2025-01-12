cmake_minimum_required(VERSION 3.5)
project(syft4golog)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Set module path for CMake
set(CMAKE_MODULE_PATH
    "${CMAKE_MODULE_PATH}"
    "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules"
)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Find packages
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)
find_package(cudd REQUIRED)
find_package(mona REQUIRED)
find_package(spot REQUIRED)

# Set Lydia-related variables
set(LYDIA_DIR ${PROJECT_SOURCE_DIR}/submodules/lydia)
set(WITH_SYFT OFF)
set(LYDIA_ENABLE_TESTS OFF)
set(LYDIA_ENABLE_BENCHMARK OFF)
add_subdirectory(${LYDIA_DIR})
set(LYDIA_LIBRARIES ${LYDIA_DIR})

# Include paths for Lydia
set(LYDIA_INCLUDE_DIR ${LYDIA_DIR}/lib/include ${LYDIA_DIR}/lib/src)
set(LYDIA_THIRD_PARTY_INCLUDE_PATH
    "${LYDIA_DIR}/third_party/spdlog/include"
    "${LYDIA_DIR}/third_party/CLI11/include"
    "/usr/local/include"
    "${LYDIA_DIR}/third_party/google/benchmark/include"
    "/usr/include"
)

# Set gologpp-related variables
set(GOLOGPP_DIR ${PROJECT_SOURCE_DIR}/submodules/gologpp)
add_subdirectory(${GOLOGPP_DIR})

# Include paths for gologpp
set(GOLOGPP_INCLUDE_DIRS
    ${GOLOGPP_DIR}/include
    ${GOLOGPP_DIR}/src
)

# External libraries and include paths
set(EXT_LIBRARIES_PATH lydia
    ${LYDIA_LIBRARIES}
    ${CUDD_LIBRARIES}
    ${MONA_DFA_LIBRARIES}
    ${MONA_BDD_LIBRARIES}
    ${MONA_MEM_LIBRARIES}
    ${SPOT_LIBRARIES}
)

set(EXT_INCLUDE_PATH
    ${LYDIA_INCLUDE_DIR}
    ${LYDIA_THIRD_PARTY_INCLUDE_PATH}
    ${GOLOGPP_INCLUDE_DIRS}
    ${CUDD_INCLUDE_DIRS}
    ${MONA_MEM_INCLUDE_DIRS}
    ${MONA_BDD_INCLUDE_DIRS}
    ${MONA_DFA_INCLUDE_DIRS}
    ${SPOT_INCLUDE_DIRS}
)

message(STATUS "EXT_LIBRARIES_PATH: ${EXT_LIBRARIES_PATH}")
message(STATUS "EXT_INCLUDE_PATH: ${EXT_INCLUDE_PATH}")

# Add the src directory
add_subdirectory(src)

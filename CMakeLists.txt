cmake_minimum_required(VERSION 3.5)
project(syft4golog)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--no-as-needed")

# Set module path for CMake
set(CMAKE_MODULE_PATH
    "${CMAKE_MODULE_PATH}"
    "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules"
)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

file(COPY ${CMAKE_SOURCE_DIR}/examples DESTINATION ${CMAKE_BINARY_DIR})

# Find packages
set(FLEX_LIBRARIES "")
set(BISON_LIBRARIES "")
find_package(cudd REQUIRED)
find_package(mona REQUIRED)
find_package(spot REQUIRED)
find_package(Syft REQUIRED)

# Set LydiaSyft-related variables
set(LYDIASYFT_DIR ${PROJECT_SOURCE_DIR}/submodules/LydiaSyft)
set(LYDIASYFT_ENABLE_TESTS OFF)
set(LYDIASYFT_ENABLE_EXAMPLES OFF)
add_subdirectory(${LYDIASYFT_DIR})

# Include paths for LydiaSyft
set(LYDIASYFT_INCLUDE_DIR ${LYDIASYFT_DIR}/src)

# Set Lydia-related variables
set(LYDIA_DIR ${LYDIASYFT_DIR}/submodules/lydia)
set(LYDIA_LIBRARIES lydia)

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
set(FLEX_LIBRARIES "" CACHE INTERNAL "Explicitly empty to prevent linking with libfl")
set(BISON_LIBRARIES "" CACHE INTERNAL "Explicitly empty to prevent linking with Bison")
add_subdirectory(${GOLOGPP_DIR})
set(GOLOGPP_LIBRARIES ${GOLOGPP_DIR}/src)

# Include paths for gologpp
set(GOLOGPP_INCLUDE_DIRS
    ${GOLOGPP_DIR}
    ${GOLOGPP_DIR}/src
    ${GOLOGPP_DIR}/src/parser
)

# set libraries for LydiaSyft
set(UTILS_INCLUDE_PATH 
    ${LYDIASYFT_INCLUDE_DIR}/utils)
set(PARSER_INCLUDE_PATH
    ${LYDIASYFT_INCLUDE_DIR}/parser)
set(SYNTHESIS_INCLUDE_PATH
    ${LYDIASYFT_INCLUDE_DIR}/synthesis/header)
set(UTILS_LIB_NAME utils)
set(PARSER_LIB_NAME parser)
set(SYNTHESIS_LIB_NAME synthesis)

# messages for LydiaSyft-related vars
# message(STATUS "LYDIASYFT_DIR: ${LYDIASYFT_DIR}")
# message(STATUS "LYDIASYFT_INCLUDE_DIR: ${LYDIASYFT_INCLUDE_DIR}")
# message(STATUS "LYDIASYFT_UTILS_INCLUDE_PATH: ${UTILS_INCLUDE_PATH}")
# message(STATUS "LYDIASYFT_PARSER_INCLUDE_PATH: ${PARSER_INCLUDE_PATH}")
# message(STATUS "LYDIASYFT_SYNTHESIS_INCLUDE_PATH: ${SYNTHESIS_INCLUDE_PATH}")
# message(STATUS "LYDIASYFT_UTILS_LIB_NAME: ${UTILS_LIB_NAME}")
# message(STATUS "LYDIASYFT_PARSER_LIB_NAME: ${PARSER_LIB_NAME}")
# message(STATUS "LYDIASYFT_SYNTHESIS_LIB_NAME: ${SYNTHESIS_LIB_NAME}")

set(EXT_INCLUDE_PATH
    ${LYDIA_INCLUDE_DIR}
    ${LYDIA_THIRD_PARTY_INCLUDE_PATH}
    ${LYDIASYFT_INCLUDE_DIR}
    ${UTILS_INCLUDE_PATH}
    ${SYNTHESIS_INCLUDE_PATH}
    ${PARSER_INCLUDE_PATH}
    ${GOLOGPP_INCLUDE_DIRS}
    ${CUDD_INCLUDE_DIRS}
    ${MONA_MEM_INCLUDE_DIRS}
    ${MONA_BDD_INCLUDE_DIRS}
    ${MONA_DFA_INCLUDE_DIRS}
    ${SPOT_INCLUDE_DIRS}
)

# Add the src directory
add_subdirectory(src)
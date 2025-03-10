cmake_minimum_required(VERSION 3.10)
project(syft4golog)

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
set(GOLOGPP_LIBRARIES ${GOLOGPP_DIR}/src ${GOLOGPP_DIR}/build)

# Include paths for gologpp
set(GOLOGPP_INCLUDE_DIRS
    ${GOLOGPP_DIR}
    ${GOLOGPP_DIR}/src
    ${GOLOGPP_DIR}/src/parser
)

# External libraries and include paths

#set(EXT_LIBRARIES_PATH lydia
set(EXT_LIBRARIES_PATH
    ${LYDIA_LIBRARIES}
    ${CUDD_LIBRARIES}
    ${GOLOGPP_LIBRARIES}
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


#Define the full paths to the source files
set(MAIN_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

# Specify the sources for the executable
set(SOURCES
    ${MAIN_SOURCE}
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${EXT_INCLUDE_PATH}
    ${PROJECT_SOURCE_DIR}/submodules/gologpp/src
    ${PROJECT_SOURCE_DIR}/submodules/gologpp/src/model
    ${PROJECT_SOURCE_DIR}/submodules/gologpp/src/parser
    ${PROJECT_SOURCE_DIR}/submodules/gologpp/src/semantics
    ${PROJECT_SOURCE_DIR}/submodules/gologpp/build
)

# Create the executable
add_executable(GologExecutable ${SOURCES})

target_link_libraries(GologExecutable
    ${EXT_LIBRARIES_PATH}
    gologpp
    #parsegolog++
)

# Install the header files
install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/#src/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h*"
)

# Install the executable
install(TARGETS GologExecutable
    RUNTIME DESTINATION bin
)
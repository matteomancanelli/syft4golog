# Specify the sources for the executable
set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/guide_stub.c
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${EXT_INCLUDE_PATH})

link_directories(/opt/eclipse_prolog/eclipse_prolog/lib/x86_64_linux)

set(FLEX_LIBRARIES "")
set(BISON_LIBRARIES "")

# Create the executable
add_executable(GologExecutable ${SOURCES})

# Explicitly list libraries to avoid linking with Flex
target_link_libraries(GologExecutable 
    ${LYDIA_LIBRARIES}
    ${CUDD_LIBRARIES}
    ${GOLOGPP_LIBRARIES}
    ${MONA_DFA_LIBRARIES}
    ${MONA_BDD_LIBRARIES}
    ${MONA_MEM_LIBRARIES}
    ${SPOT_LIBRARIES}
    golog++ 
    parsegolog++ 
    readylog++ 
    eclipse
)

# Install the header files
install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/src/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h*"
)

# Install the executable
install(TARGETS GologExecutable
    RUNTIME DESTINATION bin
)
# Generate Flex lexer
FLEX_TARGET(golog_lexer lexer.l ${CMAKE_CURRENT_SOURCE_DIR}/lex.yy.cc)
# Generate Bison parser
BISON_TARGET(golog_parser parser.yy ${CMAKE_CURRENT_SOURCE_DIR}/parser.tab.cc
             DEFINES_FILE ${CMAKE_CURRENT_SOURCE_DIR}/parser.tab.hh)
# Connect lexer to parser
ADD_FLEX_BISON_DEPENDENCY(golog_lexer golog_parser)

# Specify the sources for the executable
set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/domain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/propositional_logic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/golog_program.cpp
    ${BISON_golog_parser_OUTPUTS} 
    ${FLEX_golog_lexer_OUTPUTS}
    ${CMAKE_CURRENT_SOURCE_DIR}/golog_driver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/program_graph.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/base_golog_synthesizer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/compositional_golog_synthesizer.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${EXT_INCLUDE_PATH})

link_directories(/opt/eclipse_prolog/eclipse_prolog/lib/x86_64_linux)

# Create the executable
add_executable(GologExecutable ${SOURCES})

# Explicitly list libraries to avoid linking with Flex
target_link_libraries(GologExecutable 
    ${CUDD_LIBRARIES}
    ${MONA_DFA_LIBRARIES}
    ${MONA_BDD_LIBRARIES}
    ${MONA_MEM_LIBRARIES}
    ${UTILS_LIB_NAME} # LydiaSyft
    ${PARSER_LIB_NAME} # LydiaSyft
    ${SYNTHESIS_LIB_NAME} # LydiaSyft
    ${SPOT_LIBRARIES}
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
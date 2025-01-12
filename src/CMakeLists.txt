add_subdirectory(parser)
add_subdirectory(synthesis)

# Define the full paths to the source files
set(GOLOG_LEXER_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/golog_lexer.l)
set(GOLOG_PARSER_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/golog_parser.y)
set(GOLOG_PROGRAM_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/golog_program.cpp)
set(TRANS_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/trans.c)
set(MAIN_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

# Generate the parser and lexer
BISON_TARGET(GologParser ${GOLOG_PARSER_SOURCE} ${CMAKE_CURRENT_BINARY_DIR}/golog_parser.c DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/golog_parser.h)
FLEX_TARGET(GologLexer ${GOLOG_LEXER_SOURCE} ${CMAKE_CURRENT_BINARY_DIR}/golog_lexer.c DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/golog_lexer.h)
ADD_FLEX_BISON_DEPENDENCY(GologLexer GologParser)

# Specify the sources for the executable
set(SOURCES
    ${BISON_GologParser_OUTPUTS}
    ${FLEX_GologLexer_OUTPUTS}
    ${GOLOG_PROGRAM_SOURCE}
    ${TRANS_SOURCE}
    ${MAIN_SOURCE}
)

include_directories(${PARSER_INCLUDE_PATH} ${SYNTHESIS_INCLUDE_PATH} ${EXT_INCLUDE_PATH})
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

# Create the library for the parser
add_library(golog_parser SHARED ${GOLOG_PROGRAM_SOURCE} ${BISON_GologParser_OUTPUTS} ${FLEX_GologLexer_OUTPUTS})

# Create the executable
add_executable(GologExecutable ${SOURCES})
target_link_libraries(GologExecutable golog_parser ${EXT_LIBRARIES_PATH} ${FLEX_LIBRARIES} ${BISON_LIBRARIES} ${PARSER_LIB_NAME} ${SYNTHESIS_LIB_NAME})

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

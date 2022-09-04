file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
#PQL Files
set(srcs ${srcs}
        src/pql/Parser.cpp
        src/pql/QuerySemanticsExtractor.cpp
        src/pql/QueryEvaluator.cpp
        )
set(headers ${headers}
        src/pql/Parser.h
        src/pql/QuerySemanticsExtractor.h
        src/pql/QueryEvaluator.h
        )
add_library(spa ${srcs} ${headers})
# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)



file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")

add_executable(unit_testing ${srcs}
        "src/TestPKBStorage.cpp"
        "src/TestAST.cpp"
        "src/TestASTVisitor.cpp"
        "src/TestQueryTokenizer.cpp"
        "src/TestQueryExtractor.cpp"
        "src/TESTPKBStructures.cpp"
        "src/TestFollowsASTVisitor.cpp"
        "src/TestParentsASTVisitor.cpp"
        "src/TestModifiesASTVisitor.cpp"
        "src/TestUsesASTVisitor.cpp"
        "src/Scratch.cpp"
        "src/SPTestParser.cpp"
        "src/TestPKBPatternMatching.cpp"
        src/TestCFG.cpp
        src/TestsCallsASTVisitor.cpp)
target_link_libraries(unit_testing spa)


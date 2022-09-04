file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_library(spa ${srcs} ${headers}
        src/AST/SourceCode.h
        src/AST/Operators/RelationalOperator.h
        src/AST/Operators/Operator.h
        src/AST/Expression/Expression.h
        src/AST/Expression/NameExpression.h
        src/AST/Expression/ConstantExpression.h
        src/AST/Expression/OperatedExpression.h
        src/AST/ConditionalExpression/ConditionalExpression.h
        src/AST/ConditionalExpression/RelationalExpression.h
        src/AST/ConditionalExpression/NotCondition.h
        src/AST/ConditionalExpression/AndCondition.h
        src/AST/ConditionalExpression/OrCondition.h
        src/AST/Statement/Statement.h
        src/AST/Statement/AssignStatement.h
        src/AST/Statement/IfStatement.h
        src/AST/Statement/WhileStatement.h
        src/AST/Statement/CallStatement.h
        src/AST/Statement/PrintStatement.h
        src/AST/Statement/ReadStatement.h
        src/AST/Procedure.h
        src/AST/SourceCode.cpp
        src/AST/Procedure.cpp
        src/AST/Statement/ReadStatement.cpp
        src/AST/Statement/PrintStatement.cpp
        src/AST/Statement/CallStatement.cpp
        src/AST/Statement/WhileStatement.cpp
        src/AST/Statement/IfStatement.cpp
        src/AST/Statement/AssignStatement.cpp
        src/AST/Expression/NameExpression.cpp
        src/AST/Expression/ConstantExpression.cpp
        src/AST/Expression/OperatedExpression.cpp
        src/AST/ConditionalExpression/RelationalExpression.cpp
        src/AST/ConditionalExpression/NotCondition.cpp
        src/AST/ConditionalExpression/AndCondition.cpp
        src/AST/ConditionalExpression/OrCondition.cpp)
# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)



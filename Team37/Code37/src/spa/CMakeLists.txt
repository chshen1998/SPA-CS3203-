file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")

add_library(spa ${srcs} ${headers}
        src/AST/TNode.h
        src/AST/SourceCode.h
        src/AST/SourceCode.cpp
        src/AST/Procedure.h
        src/AST/Procedure.cpp

        src/AST/Statement/Statement.h
        src/AST/Statement/AssignStatement.h
        src/AST/Statement/AssignStatement.cpp
        src/AST/Statement/IfStatement.h
        src/AST/Statement/IfStatement.cpp
        src/AST/Statement/WhileStatement.h
        src/AST/Statement/WhileStatement.cpp
        src/AST/Statement/CallStatement.h
        src/AST/Statement/CallStatement.cpp
        src/AST/Statement/PrintStatement.h
        src/AST/Statement/PrintStatement.cpp
        src/AST/Statement/ReadStatement.h
        src/AST/Statement/ReadStatement.cpp

        src/AST/Expression/Expression.h
        src/AST/Expression/RelationalFactor/RelationalFactor.h
        src/AST/Expression/RelationalFactor/NameExpression.h
        src/AST/Expression/RelationalFactor/NameExpression.cpp
        src/AST/Expression/RelationalFactor/ConstantExpression.h
        src/AST/Expression/RelationalFactor/ConstantExpression.cpp
        src/AST/Expression/RelationalFactor/OperatedExpression.h
        src/AST/Expression/RelationalFactor/OperatedExpression.cpp
        src/AST/Expression/ConditionalExpression/ConditionalExpression.h
        src/AST/Expression/ConditionalExpression/RelationalExpression.h
        src/AST/Expression/ConditionalExpression/RelationalExpression.cpp
        src/AST/Expression/ConditionalExpression/NotCondition.h
        src/AST/Expression/ConditionalExpression/NotCondition.cpp
        src/AST/Expression/ConditionalExpression/AndCondition.h
        src/AST/Expression/ConditionalExpression/AndCondition.cpp
        src/AST/Expression/ConditionalExpression/OrCondition.h
        src/AST/Expression/ConditionalExpression/OrCondition.cpp

        src/AST/Operators/Operator.h
        src/AST/Operators/RelationalOperator.h

        src/AST/ASTVisitor/ASTVisitor.h

        src/QPS/QPS.h
        src/QPS/QueryExtractor.h
        src/QPS/QueryEvaluator.h
        src/QPS/QueryTokenizer.h

        src/SP/Utils.h
        src/SP/ParserVisitor.h
        src/SP/SP.h
        src/SP/TokeniserVisitor.h
        src/SP/Tokenizer.h
        src/SP/Visitor.h
        src/SP/Utils.cpp
        src/SP/ParserVisitor.cpp
        src/SP/SP.cpp
        src/SP/TokeniserVisitor.cpp
        src/SP/Tokenizer.cpp
        src/SP/Visitor.cpp

        src/QPS/QPS.cpp
        src/QPS/QueryExtractor.cpp
        src/QPS/QueryEvaluator.cpp
        src/QPS/QueryTokenizer.cpp

        src/PKB/PKB.h
        src/PKB/PKB.cpp
        src/PKB/QueryServicer.h
        src/PKB/QueryServicer.cpp
        src/PKB/Storage.h
        src/PKB/Storage.cpp
        src/PKB/ElementType.h)

# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)



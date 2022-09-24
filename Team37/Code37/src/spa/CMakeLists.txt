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

        "src/QPS/Validators/ClauseValidator.h"
        "src/QPS/Validators/DeclarationValidator.h"
        "src/QPS/Validators/FollowsValidator.h"
        "src/QPS/Validators/PatternValidator.h"
        "src/QPS/Validators/ParentValidator.h"
        "src/QPS/Validators/SelectValidator.h"
        "src/QPS/Validators/ModifiesValidator.h"
        "src/QPS/Validators/UsesValidator.h"
        "src/QPS/Validators/ValidatorUtils.h"
        src/QPS/QPS.h
        src/QPS/QueryExtractor.h
        src/QPS/QueryEvaluator.h
        src/QPS/QueryTokenizer.h

        src/SP/Utilities/Utils.h
        src/SP/SP.h
        src/SP/Tokenizer.h
        src/SP/Utilities/Utils.cpp
        src/SP/SP.cpp
        src/SP/Tokenizer.cpp

        "src/QPS/Validators/ClauseValidator.cpp"
        "src/QPS/Validators/DeclarationValidator.cpp"
        "src/QPS/Validators/FollowsValidator.cpp"
        "src/QPS/Validators/PatternValidator.cpp"
        "src/QPS/Validators/ParentValidator.cpp"
        "src/QPS/Validators/SelectValidator.cpp"
        "src/QPS/Validators/ModifiesValidator.cpp"
        "src/QPS/Validators/UsesValidator.cpp"
        "src/QPS/Validators/ValidatorUtils.cpp"

        src/QPS/QPS.cpp
        src/QPS/QueryExtractor.cpp
        src/QPS/QueryEvaluator.cpp
        src/QPS/QueryTokenizer.cpp
        src/QPS/QueryValidator.cpp

        src/PKB/PKB.h
        src/PKB/PKB.cpp
        src/PKB/QueryServicer.h
        src/PKB/QueryServicer.cpp
        src/PKB/Storage.h
        src/PKB/Storage.cpp
        src/SP/Utilities/Keywords.cpp
        src/SP/Utilities/Keywords.h
        src/SP/Parser.cpp
        src/SP/Parser.h
        src/SP/InvalidSyntaxException.h
        src/PKB/Types/StatementType.h
        src/AST/ASTVisitor/ExtractGeneralASTVisitor.cpp
        src/AST/ASTVisitor/ExtractGeneralASTVisitor.h
        src/PKB/Structures/Array2D.h
        src/PKB/Structures/Array2D.cpp
        src/PKB/Structures/StatementVariableStorage.h
        src/PKB/Structures/StatementVariableStorage.cpp
        src/PKB/Structures/Array2D.cpp
        src/AST/ASTVisitor/ExtractFollowsASTVisitor.cpp
        src/AST/ASTVisitor/ExtractFollowsASTVisitor.h
        src/AST/ASTVisitor/ExtractParentsASTVisitor.cpp
        src/AST/ASTVisitor/ExtractParentsASTVisitor.h
        src/PKB/Types/StmtStmtRelationType.h
        src/AST/ASTVisitor/ExtractParentsASTVisitor.h
        src/PKB/Types/StmtStmtRelationType.h
        src/AST/ASTVisitor/ExtractModifiesASTVisitor.cpp
        src/AST/ASTVisitor/ExtractModifiesASTVisitor.h
        src/AST/ASTVisitor/ExtractUsesASTVisitor.cpp
        src/AST/ASTVisitor/ExtractUsesASTVisitor.h  "src/QPS/Structures/PqlError.h" "src/QPS/Types/ErrorType.h" "src/QPS/Structures/PqlQuery.h" "src/QPS/Types/TokenType.h" "src/QPS/Structures/PqlToken.h" "src/QPS/Types/TokenType.cpp" "src/QPS/Types/ErrorType.cpp" "src/QPS/Structures/PqlToken.cpp")

# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)



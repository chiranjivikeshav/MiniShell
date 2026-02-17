#ifndef AST_EXECUTOR_H
#define AST_EXECUTOR_H

#include "ASTNode.h"

class ASTExecutor
{
    public:
    int execute(const ASTNode* node);
    int executeSubshell(const ASTNode* node);
    int executeBackground(const ASTNode* node);
};

#endif
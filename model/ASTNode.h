#ifndef ASTNODE_H
#define ASTNODE_H

#include <memory>

enum class NodeType {
    COMMAND,     // command
    PIPE,        // |
    SEQUENCE,    // ;
    AND,         // &&
    OR,          // ||
    SUBSHELL,    // ()
    BACKGROUND   // &
};

struct ASTNode
{
    NodeType type;
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;

    virtual ~ASTNode() = default;
};

#endif

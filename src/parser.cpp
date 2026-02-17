#include "parser.h"
#include "command.h"
#include <sstream>
#include "subshellNode.h"

Parser::Parser(std::vector<Token>& t):tokens(t)
{

}

std::unique_ptr<ASTNode> Parser::parse()
{
    return parseSequence();
}

std::unique_ptr<ASTNode> Parser::parseSequence() {
    auto node = parseLogical();

    while (true) {
        if (match(TokenType::SEQUENCE))
        {
            auto right = parseLogical();
            auto parent = std::make_unique<ASTNode>();
            parent->type = NodeType::SEQUENCE;
            parent->left = std::move(node);
            parent->right = std::move(right);
            node = std::move(parent);
        }
        else if (match(TokenType::BACKGROUND))
        {
            auto bg = std::make_unique<ASTNode>();
            bg->type = NodeType::BACKGROUND;
            bg->left = std::move(node);
            node = std::move(bg);

            auto right = parseLogical();
            auto parent = std::make_unique<ASTNode>();
            parent->type = NodeType::SEQUENCE;
            parent->left = std::move(node);
            parent->right = std::move(right);
            node = std::move(parent);
        }
        else
        {
            break;
        }
    }
    return node;
}

std::unique_ptr<ASTNode> Parser::parseLogical() {
    auto node = parsePipeline();

    while (match(TokenType::AND) || match(TokenType::OR)) {
        NodeType type = tokens[pos-1].type == TokenType::AND
                        ? NodeType::AND
                        : NodeType::OR;

        auto right = parsePipeline();
        auto parent = std::make_unique<ASTNode>();
        parent->type = type;
        parent->left = std::move(node);
        parent->right = std::move(right);
        node = std::move(parent);
    }
    return node;
}

std::unique_ptr<ASTNode> Parser::parsePipeline() {
    auto node = parsePrimary();

    while (match(TokenType::PIPE)) {
        auto right = parsePrimary();
        auto parent = std::make_unique<ASTNode>();
        parent->type = NodeType::PIPE;
        parent->left = std::move(node);
        parent->right = std::move(right);
        node = std::move(parent);
    }
    return node;
}

std::unique_ptr<ASTNode> Parser::parsePrimary() {
    if (match(TokenType::LPAREN)) {
        auto inner = parseSequence();

        if (!match(TokenType::RPAREN)) {
            throw std::runtime_error("expected ')'");
        }

        return std::make_unique<SubshellNode>(std::move(inner));
    }

    return parseCommand();
}

std::unique_ptr<ASTNode> Parser::parseCommand() {
    auto cmd = std::make_unique<Command>();

    while (peek().type == TokenType::WORD) {
        cmd->args.push_back(peek().value);
        pos++;
    }

    if (!cmd->args.empty()) {
        cmd->name = cmd->args[0];
    }

    while (peek().type == TokenType::REDIRECT) {
        Redirection rd;
        std::string op = peek().value;
        pos++;

        rd.file = peek().value;
        setRedirectionType(op, rd);
        pos++;

        cmd->redirections.push_back(rd);
    }

    return cmd;
}

void Parser::setRedirectionType(const std::string& arg, Redirection& rd)
{
    if (arg == ">") {
        rd.type = RedirectType::OUTPUT;
    }
    else if (arg == ">>") {
        rd.type = RedirectType::APPEND;
    }
    else if (arg == "<") {
        rd.type = RedirectType::INPUT;
    }
    else if (arg == "2>") {
        rd.type = RedirectType::STDERR;
    }
}

bool Parser::match(TokenType type) {
    if (peek().type == type) {
        pos++;
        return true;
    }
    return false;
}

const Token& Parser::peek() {
    return tokens[pos];
}

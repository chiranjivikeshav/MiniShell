#include "token.h"
#include <sstream>
#include <vector>

std::vector<Token> Tokenizer::tokenize(const std::string& input) {
    std::istringstream iss(input);
    std::vector<Token> tokens;
    std::string s;

    while (iss >> s) {
        if (s == "|") tokens.push_back({TokenType::PIPE, s});
        else if (s == "&&") tokens.push_back({TokenType::AND, s});
        else if (s == "||") tokens.push_back({TokenType::OR, s});
        else if (s == ";") tokens.push_back({TokenType::SEQUENCE, s});
        else if (s == "(") tokens.push_back({TokenType::LPAREN, s});
        else if (s == ")") tokens.push_back({TokenType::RPAREN, s});
        else if (s == "&") tokens.push_back({TokenType::BACKGROUND, s});
        else if (s == ">" || s == ">>" || s == "<" || s == "2>")
            tokens.push_back({TokenType::REDIRECT, s});
        else
            tokens.push_back({TokenType::WORD, s});
    }

    tokens.push_back({TokenType::END, ""});
    return tokens;
}

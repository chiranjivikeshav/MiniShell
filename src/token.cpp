#include "token.h"
#include <cctype>
#include <vector>

std::vector<Token> Tokenizer::tokenize(const std::string& input)
{
    std::vector<Token> tokens;
    size_t i = 0;

    while (i < input.size())
    {
        if (std::isspace(input[i])) {
            i++;
            continue;
        }

        if (parseTwoCharOperator(input, i, tokens))
            continue;

        if (parseOneCharOperator(input, i, tokens))
            continue;

        parseWord(input, i, tokens);
    }

    tokens.push_back({TokenType::END, ""});
    return tokens;
}

bool Tokenizer::parseTwoCharOperator(const std::string& input, size_t& i, std::vector<Token>& tokens)
{
    if (i + 1 >= input.size())
        return false;

    std::string op = input.substr(i,2);

    if (op == "&&") {
        tokens.push_back({TokenType::AND,"&&"});
    }
    else if (op == "||") {
        tokens.push_back({TokenType::OR,"||"});
    }
    else if (op == ">>") {
        tokens.push_back({TokenType::REDIRECT,">>"});
    }
    else if (op == "2>") {
        tokens.push_back({TokenType::REDIRECT,"2>"});
    }
    else {
        return false;
    }

    i += 2;
    return true;
}

bool Tokenizer::parseOneCharOperator(const std::string& input, size_t& i, std::vector<Token>& tokens)
{
    switch (input[i])
    {
    case '|':
        tokens.push_back({TokenType::PIPE,"|"});
        break;

    case '>':
        tokens.push_back({TokenType::REDIRECT,">"});
        break;

    case '<':
        tokens.push_back({TokenType::REDIRECT,"<"});
        break;

    case ';':
        tokens.push_back({TokenType::SEQUENCE,";"});
        break;

    case '(':
        tokens.push_back({TokenType::LPAREN,"("});
        break;

    case ')':
        tokens.push_back({TokenType::RPAREN,")"});
        break;

    case '&':
        tokens.push_back({TokenType::BACKGROUND,"&"});
        break;

    default:
        return false;
    }

    i++;
    return true;
}

void Tokenizer::parseWord(const std::string& input, size_t& i, std::vector<Token>& tokens)
{
    std::string word;

    auto isOperator = [](char c){
        return std::string("|&;()<>").find(c) != std::string::npos;
    };

    while (i < input.size() &&
           !std::isspace(input[i]) &&
           !isOperator(input[i]))
    {
        word += input[i];
        i++;
    }

    tokens.push_back({TokenType::WORD,word});
}
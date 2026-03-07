#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <vector>

enum class TokenType {
    WORD,
    PIPE,       // |
    AND,        // &&
    OR,         // ||
    SEQUENCE,   // ;
    REDIRECT,   // > >> < 2>
    LPAREN,     // (
    RPAREN,     // )
    BACKGROUND, // &
    END
};

struct Token {
    TokenType type;
    std::string value;
};

class Tokenizer
{
    public:
    std::vector<Token> tokenize(const std::string& input);
    static bool parseTwoCharOperator(const std::string& input, size_t& i, std::vector<Token>& tokens);
    static bool parseOneCharOperator(const std::string& input, size_t& i, std::vector<Token>& tokens);
    static void parseWord(const std::string& input, size_t& i, std::vector<Token>& tokens);
};
#endif
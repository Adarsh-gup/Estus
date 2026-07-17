#pragma once
#include <string>
#include <vector>
#include <variant>
#include <unordered_map>
#include "../Token/Token.hpp"

class Scanner {
private:
    const std::string m_source {""};
    std::vector<Token> m_tokens {};
    int m_start   {0};
    int m_current {0};
    int m_line    {1};
     
    inline static const std::unordered_map<std::string, TokenType> keywords {
        {"and",    TokenType::AND},
        {"class",  TokenType::CLASS},
        {"else",   TokenType::ELSE},
        {"false",  TokenType::FALSE},
        {"for",    TokenType::FOR},
        {"fun",    TokenType::FUN},
        {"if",     TokenType::IF},
        {"nil",    TokenType::NIL},
        {"or",     TokenType::OR},
        {"print",  TokenType::PRINT},
        {"return", TokenType::RETURN},
        {"super",  TokenType::SUPER},
        {"this",   TokenType::THIS},
        {"true",   TokenType::TRUE},
        {"var",    TokenType::VAR},
        {"while",  TokenType::WHILE},
    };
    // Internal helpers — private
    bool isDigit(char c);
    bool isAlpha(char c);
    bool isAlphaNumeric(char c);
    bool EndofSource();
    char advance();
    void addToken(TokenType type);
    void addToken(TokenType type, std::variant<std::monostate, double, std::string> literal);
    bool match(char expected);
    char peek();
    char peekNext();
    void string();
    void numbers();
    void identifier();
    void scanToken();

public:
    Scanner(const std::string& source) : m_source{source} {}
    std::vector<Token> scanTokens();
};

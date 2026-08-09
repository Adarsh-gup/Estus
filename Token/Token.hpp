#pragma once
#include <iostream>
#include <string>
#include <string_view>
#include "tokenType.hpp"
#include <variant>

// Forward declaration — defined in Token.cpp
std::string tokenToString(const TokenType& type);

class Token {
public: 
TokenType m_type;
std::string m_lexeme {"???"};
std::variant<std::monostate, double, bool, std::string> m_literal;
int m_line {0};

Token(TokenType type, std::string lexeme,
     std::variant<std::monostate, double, bool, std::string> literal, int line) :
    m_type{type},
    m_lexeme{lexeme},
    m_literal{literal}, m_line{line} 
{} 
friend std::ostream& operator<<(std::ostream& out, const Token& token);
void toString() const;
const std::string& getLexeme() const { return m_lexeme; }
};


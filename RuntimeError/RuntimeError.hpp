#pragma once
#include <stdexcept>
#include <string>
#include "../Token/Token.hpp"

class RuntimeError : public std::runtime_error {
public:
    Token m_token;
    RuntimeError(Token token, const std::string& message)
        : std::runtime_error(message), m_token(std::move(token)) {}
};
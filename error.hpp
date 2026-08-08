#pragma once
#include <string>
#include "Token/Token.hpp"

// Global flag — set true whenever any error is reported
extern bool hadError;

// Called by both error() overloads; prints to stdout and sets hadError
void report(int line, const std::string& where, const std::string& message);

namespace PrintError {
    // Lexer / scanner errors — just a line number
    void error(int line, const std::string& message);

    // Parser errors — carries the offending token for richer messages
    void error(const Token& token, const std::string& message);
}
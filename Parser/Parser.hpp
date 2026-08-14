#pragma once
#include <vector>
#include <stdexcept>
#include <memory>
#include "../Expressions/expr.hpp"
#include "../Statement/Statement.hpp"
#include "../Token/Token.hpp"
#include "../error.hpp"

class Parser {
private:
    std::vector<Token> m_tokens{};
    int m_current{0};

    // Nested exception type used for error recovery
    class ParseError : public std::runtime_error {
    public:
        ParseError() : std::runtime_error("") {}
    };

    // helpers
    bool      isAtEnd();
    Token     peek();
    Token     previous();
    Token     advance();
    bool      check(TokenType type);
    bool      match(std::initializer_list<TokenType> types);
    Token     consume(TokenType type, const std::string& message);
    ParseError error(Token token, const std::string& message);
    void      synchronize();

    //Grammar rules (recursive descent)
    std::unique_ptr<Expr> expression();
    std::unique_ptr<Stmt> statement();
    std::unique_ptr<Stmt> printStatement();
    std::unique_ptr<Stmt> expressionStatement();
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> primary();

public:
    explicit Parser(std::vector<Token> tokens) : m_tokens{std::move(tokens)} {}

    std::vector<std::unique_ptr<Stmt>> parse();
};

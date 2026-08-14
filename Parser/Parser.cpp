#include "Parser.hpp"

// helpers

bool Parser::isAtEnd() {
    return peek().m_type == TokenType::END_OF_FILE;
}

Token Parser::peek() {
    return m_tokens[m_current];
}

Token Parser::previous() {
    return m_tokens[m_current - 1];
}

Token Parser::advance() {
    if (!isAtEnd()) m_current++;
    return previous();
}

bool Parser::check(TokenType type) {
    if (isAtEnd()) return false;
    return peek().m_type == type;
}

bool Parser::match(std::initializer_list<TokenType> types) {
    for (const auto& type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();
    throw error(peek(), message);
}

Parser::ParseError Parser::error(Token token, const std::string& message) {
    PrintError::error(token, message);
    return ParseError();
}

void Parser::synchronize() {
    advance();
    while (!isAtEnd()) {
        if (previous().m_type == TokenType::SEMICOLON) return;

        switch (peek().m_type) {
            case TokenType::CLASS: case TokenType::FUN: case TokenType::LET: case TokenType::FOR:
            case TokenType::IF:    case TokenType::WHILE: case TokenType::PRINT: case TokenType::RETURN:
                return;
            default:
                break;
        }
        advance();
    }
}

//Grammar rules (recursive descent) 
std::unique_ptr<Expr> Parser::expression() {
    return equality();
}

std::unique_ptr<Stmt> Parser::statement() {
    if (match({TokenType::PRINT})) return printStatement();
    return expressionStatement();
}

std::unique_ptr<Stmt> Parser::declaration() {
    try {
        if (match({TokenType::LET})) return varDeclaration();
        return statement();
    }
    catch (const ParseError&) {
        synchronize();
        return nullptr;
    }
}

std::unique_ptr<Stmt> Parser::printStatement() {
    std::unique_ptr<Expr> value = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after value.");
    return std::make_unique<PrintStmt>(std::move(value));
}

std::unique_ptr<Stmt> Parser::varDeclaration() {
    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
    std::unique_ptr<Expr> initializer = nullptr;
    if (match({TokenType::EQUAL})) {
        initializer = expression();
    }
    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_unique<Let>(std::move(name), std::move(initializer));
}

std::unique_ptr<Stmt> Parser::expressionStatement() {
    std::unique_ptr<Expr> expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after expression.");
    return std::make_unique<ExpressionStmt>(std::move(expr));
}
std::unique_ptr<Expr> Parser::equality() {
    std::unique_ptr<Expr> expr = comparison();
    while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
        Token op = previous();
        std::unique_ptr<Expr> right = comparison();
        expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::comparison() {
    std::unique_ptr<Expr> expr = term();
    while (match({TokenType::GREATER, TokenType::GREATER_EQUAL,
                  TokenType::LESS,    TokenType::LESS_EQUAL}))
    {
        Token op = previous();
        std::unique_ptr<Expr> right = term();
        expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::term() {
    std::unique_ptr<Expr> expr = factor();
    while (match({TokenType::MINUS, TokenType::PLUS})) {
        Token op = previous();
        std::unique_ptr<Expr> right = factor();
        expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::factor() {
    std::unique_ptr<Expr> expr = unary();
    while (match({TokenType::SLASH, TokenType::STAR})) {
        Token op = previous();
        std::unique_ptr<Expr> right = unary();
        expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::unary() {
    if (match({TokenType::BANG, TokenType::MINUS})) {
        Token op = previous();
        std::unique_ptr<Expr> right = unary();
        return std::make_unique<Unary>(op, std::move(right));
    }
    return primary();
}

std::unique_ptr<Expr> Parser::primary() {
    if (match({TokenType::FALSE})) return std::make_unique<Literal>(false);
    if (match({TokenType::TRUE}))  return std::make_unique<Literal>(true);
    if (match({TokenType::NIL}))   return std::make_unique<Literal>(std::monostate{});

    if (match({TokenType::NUMBER, TokenType::STRING})) {
        return std::make_unique<Literal>(previous().m_literal);
    }
    if (match({TokenType::IDENTIFIER})) {
        return std::make_unique<Variable>(previous());
    }
    if (match({TokenType::LEFT_PAREN})) {
        std::unique_ptr<Expr> expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_unique<Grouping>(std::move(expr));
    }

    throw error(peek(), "Expect expression.");
}

// Public entry point 

std::vector<std::unique_ptr<Stmt>> Parser::parse() {
    std::vector<std::unique_ptr<Stmt>> statements;
    try {
        while (!isAtEnd()) {
            statements.push_back(declaration());
        }
        return statements;
    } catch (const ParseError&) {
        return {};
    }
}
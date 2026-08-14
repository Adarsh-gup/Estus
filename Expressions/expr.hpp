#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <variant>
#include "../Token/Token.hpp"

// Forward declarations
class Binary;
class Grouping;
class Literal;
class Unary;
class Variable;

using Value = std::variant<std::monostate, double, bool, std::string>;

//  Visitor interface  (single return type: Value)

class ExprVisitor {
public:
    virtual ~ExprVisitor() = default;
    virtual Value visitBinary(Binary& binary)       = 0;
    virtual Value visitGrouping(Grouping& grouping) = 0;
    virtual Value visitLiteral(Literal& literal)    = 0;
    virtual Value visitUnary(Unary& unary)          = 0;
    virtual Value visitVariable(Variable& variable) = 0;
};

// new interface for Interpreter

class ExprEvaluator {  
public:
    virtual ~ExprEvaluator() = default;
    virtual Value visitBinary(Binary& binary) = 0;
    virtual Value visitGrouping(Grouping& grouping) = 0;
    virtual Value visitUnary(Unary& unary) = 0;
    virtual Value visitLiteral(Literal& literal) = 0;
    virtual Value visitVariable(Variable& variable) = 0;
};

//  Abstract base Expr

class Expr {
public:
    virtual ~Expr() = default;
    virtual Value accept(ExprVisitor& visitor) = 0;
    virtual Value accept(ExprEvaluator& visitor) = 0;
};


class Binary : public Expr {
public:
    const std::unique_ptr<Expr> m_left;
    const Token                 m_op;
    const std::unique_ptr<Expr> m_right;

    Binary(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right)
        : m_left(std::move(left)), m_op(std::move(op)), m_right(std::move(right)) {}

    Value accept(ExprVisitor& visitor) override {
        return visitor.visitBinary(*this);
    }

    Value accept(ExprEvaluator& visitor) override {
        return visitor.visitBinary(*this);
    }
};

class Grouping : public Expr {
public:
    const std::unique_ptr<Expr> m_expr;

    explicit Grouping(std::unique_ptr<Expr> expr)
        : m_expr(std::move(expr)) {}

    Value accept(ExprVisitor& visitor) override {
        return visitor.visitGrouping(*this);
    }
    Value accept(ExprEvaluator& visitor) override {
        return visitor.visitGrouping(*this);
    }
};

class Literal : public Expr {
public:
    const Value m_value;

    explicit Literal(Value value)
        : m_value(std::move(value)) {}

    Value accept(ExprVisitor& visitor) override {
        return visitor.visitLiteral(*this);
    }
    Value accept(ExprEvaluator& visitor) override {
        return visitor.visitLiteral(*this);
    }
};

class Unary : public Expr {
public:
    const Token                 m_op;
    const std::unique_ptr<Expr> m_right;

    Unary(Token op, std::unique_ptr<Expr> right)
        : m_op(std::move(op)), m_right(std::move(right)) {}

    Value accept(ExprVisitor& visitor) override {
        return visitor.visitUnary(*this);
    }
    Value accept(ExprEvaluator& visitor) override {
        return visitor.visitUnary(*this);
    }
};

class Variable : public Expr {
public:
    const Token m_name;

    explicit Variable(Token name)
        : m_name(std::move(name)) {}

    Value accept(ExprVisitor& visitor) override {
        return visitor.visitVariable(*this);
    }
    Value accept(ExprEvaluator& visitor) override {
        return visitor.visitVariable(*this);
    }
};

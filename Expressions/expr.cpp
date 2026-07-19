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

// ─────────────────────────────────────────────
//  Visitor interface
// ─────────────────────────────────────────────
class ExprVisitor {
public:
    virtual ~ExprVisitor() = default;
    virtual void visitBinary(Binary& binary)     = 0;
    virtual void visitGrouping(Grouping& grouping) = 0;
    virtual void visitLiteral(Literal& literal)  = 0;
    virtual void visitUnary(Unary& unary)        = 0;
};


class Expr {
public:
    virtual ~Expr() = default;
    virtual void accept(ExprVisitor& visitor) = 0;
};


class Binary : public Expr {
public:
    const std::unique_ptr<Expr> m_left;
    const Token m_op;
    const std::unique_ptr<Expr> m_right;

    Binary(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right)
        : m_left(std::move(left)), m_op(std::move(op)), m_right(std::move(right)) {}

    void accept(ExprVisitor& visitor) override {
        visitor.visitBinary(*this);
    }
};

class Grouping : public Expr {
public:
    const std::unique_ptr<Expr> m_expr;

    explicit Grouping(std::unique_ptr<Expr> expr)
        : m_expr(std::move(expr)) {}

    void accept(ExprVisitor& visitor) override {
        visitor.visitGrouping(*this);
    }
};

class Literal : public Expr {
public:
    const std::variant<std::monostate, double, std::string> m_value;

    explicit Literal(std::variant<std::monostate, double, std::string> value)
        : m_value(std::move(value)) {}

    void accept(ExprVisitor& visitor) override {
        visitor.visitLiteral(*this);
    }
};

class Unary : public Expr {
public:
    const Token m_op;
    const std::unique_ptr<Expr> m_right;

    Unary(Token op, std::unique_ptr<Expr> right)
        : m_op(std::move(op)), m_right(std::move(right)) {}

    void accept(ExprVisitor& visitor) override {
        visitor.visitUnary(*this);
    }
};

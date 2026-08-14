#pragma once

#include <memory>
#include "../Expressions/expr.hpp"

// Forward declarations
class ExpressionStmt;
class PrintStmt;
class Let;

class StmtVisitor {
public:
    virtual ~StmtVisitor() = default;
    virtual void visitExpressionStmt(ExpressionStmt& stmt) = 0;
    virtual void visitPrintStmt(PrintStmt& stmt) = 0;
    virtual void visitLetStmt(Let& stmt) = 0;
};

class Stmt {
public:
    virtual ~Stmt() = default;
    virtual void accept(StmtVisitor& visitor) = 0;
};

class Let : public Stmt {
public:
    const Token m_name;
    const std::unique_ptr<Expr> m_initializer;

    Let(Token name, std::unique_ptr<Expr> initializer)
        : m_name(std::move(name)), m_initializer(std::move(initializer)) {}

    void accept(StmtVisitor& visitor) override {
        visitor.visitLetStmt(*this);
    }
};
class ExpressionStmt : public Stmt {
public:
    const std::unique_ptr<Expr> m_expression;

    explicit ExpressionStmt(std::unique_ptr<Expr> expression)
        : m_expression(std::move(expression)) {}

    void accept(StmtVisitor& visitor) override {
        visitor.visitExpressionStmt(*this);
    }
};

class PrintStmt : public Stmt {
public:
    const std::unique_ptr<Expr> m_expression;

    explicit PrintStmt(std::unique_ptr<Expr> expression)
        : m_expression(std::move(expression)) {}

    void accept(StmtVisitor& visitor) override {
        visitor.visitPrintStmt(*this);
    }
};

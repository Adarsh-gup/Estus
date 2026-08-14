#pragma once

#include <memory>
#include "../Expressions/expr.hpp"

// Forward declarations
class ExpressionStmt;
class PrintStmt;

class StmtVisitor {
public:
    virtual ~StmtVisitor() = default;
    virtual void visitExpressionStmt(ExpressionStmt& stmt) = 0;
    virtual void visitPrintStmt(PrintStmt& stmt) = 0;
};

class Stmt {
public:
    virtual ~Stmt() = default;
    virtual void accept(StmtVisitor& visitor) = 0;
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

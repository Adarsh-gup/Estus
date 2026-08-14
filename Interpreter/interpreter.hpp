#pragma once

#include "../Expressions/expr.hpp"
#include "../Statement/Statement.hpp"
#include "../Environment/Environment.hpp"
#include "../RuntimeError/RuntimeError.hpp"
#include <string>
#include <vector>

class Interpreter : public ExprEvaluator, public StmtVisitor {
private:
    // helpers 
    Value evaluate(Expr& expr);
    void execute(Stmt& stmt);
    void checkNumberOperand(const Token& op, const Value& operand);
    void checkNumberOperands(const Token& op, const Value& left, const Value& right);
    std::string stringify(const Value& value);
    Environment m_environment{};

public:
    // public entry point
    void interpret(std::vector<std::unique_ptr<Stmt>>& statements);

    // visitor implementations
    Value visitLiteral(Literal& literal) override;
    Value visitGrouping(Grouping& grouping) override;
    Value visitUnary(Unary& unary) override;
    Value visitBinary(Binary& binary) override;
    Value visitVariable(Variable& variable) override;

    void visitExpressionStmt(ExpressionStmt& stmt) override;
    void visitPrintStmt(PrintStmt& stmt) override;
    void visitLetStmt(Let& stmt) override;
};

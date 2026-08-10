#pragma once

#include "../Expressions/expr.hpp"
#include "../RuntimeError/RuntimeError.hpp"
#include <string>

class Interpreter : public ExprEvaluator {
private:
    // helpers 
    Value evaluate(Expr& expr);
    void checkNumberOperand(const Token& op, const Value& operand);
    void checkNumberOperands(const Token& op, const Value& left, const Value& right);
    std::string stringify(const Value& value);

public:
    // public entry point
    void interpret(Expr& expression);

    // visitor implementations
    Value visitLiteral(Literal& literal) override;
    Value visitGrouping(Grouping& grouping) override;
    Value visitUnary(Unary& unary) override;
    Value visitBinary(Binary& binary) override;
};

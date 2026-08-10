#pragma once
#include <string>
#include <sstream>
#include <memory>
#include <variant>
#include "../Expressions/expr.hpp"

class AstPrinter : public ExprVisitor {
public:
    // Entry point — returns a plain std::string by unwrapping the Value
    std::string print(Expr& expr);

    Value visitBinary  (Binary&   binary)   override;
    Value visitGrouping(Grouping& grouping) override;
    Value visitLiteral (Literal&  literal)  override;
    Value visitUnary   (Unary&    unary)    override;

private:
    // Wraps an operator name and a list of sub-expressions in parentheses
    Value parenthesize(const std::string& op,
                       std::initializer_list<Expr*> exprs);
};

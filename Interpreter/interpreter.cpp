#include "../Expressions/expr.hpp"
#include <stdexcept>

class Interpreter : public ExprEvaluator {
private:
    Value evaluate(Expr& expr) {
        return expr.accept(*this);
    }

public:
    Value visitLiteral(Literal& literal) override {
        return literal.m_value;           // m_value is a Value, not a pointer
    }

    Value visitGrouping(Grouping& grouping) override {
        return evaluate(*grouping.m_expr);
    }

    Value visitUnary(Unary& unary) override {
        Value right = evaluate(*unary.m_right);

        switch (unary.m_op.m_type) {
            case TokenType::MINUS:
                return -std::get<double>(right);

            case TokenType::BANG:
                if (std::holds_alternative<bool>(right))
                    return !std::get<bool>(right);
                if (std::holds_alternative<std::monostate>(right))
                    return true;   // !nil == true
                return false;      // numbers/strings are truthy

            default:
                std::cout << "Unknown Unary Operator" <<"\n";
        }
    }

    Value visitBinary(Binary& binary) override {
        Value left  = evaluate(*binary.m_left);   // dereference unique_ptr
        Value right = evaluate(*binary.m_right);  // dereference unique_ptr

        switch (binary.m_op.m_type) {
            // Arithmetic
            case TokenType::MINUS:
                return std::get<double>(left) - std::get<double>(right);
            case TokenType::STAR:
                return std::get<double>(left) * std::get<double>(right);
            case TokenType::SLASH:
                return std::get<double>(left) / std::get<double>(right);
            case TokenType::PLUS:
                if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right))
                    return std::get<double>(left) + std::get<double>(right);
                if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right))
                    return std::get<std::string>(left) + std::get<std::string>(right);
                throw std::runtime_error("Operands must be two numbers or two strings.");

            // Comparison
            case TokenType::GREATER:
                return std::get<double>(left) > std::get<double>(right);
            case TokenType::GREATER_EQUAL:
                return std::get<double>(left) >= std::get<double>(right);
            case TokenType::LESS:
                return std::get<double>(left) < std::get<double>(right);
            case TokenType::LESS_EQUAL:
                return std::get<double>(left) <= std::get<double>(right);

            // Equality (works across any Value type)
            case TokenType::EQUAL_EQUAL:
                return left == right;
            case TokenType::BANG_EQUAL:
                return left != right;

            default:
                std::cout << "Unknown Unary Operator" <<"\n";
        }
    }
};
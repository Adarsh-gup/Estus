#include "../Expressions/expr.hpp"
#include "../RuntimeError/RuntimeError.hpp"
#include <iostream>
#include <string>
#include <stdexcept>

class Interpreter : public ExprEvaluator {
private:
    // helpers 

    Value evaluate(Expr& expr) {
        return expr.accept(*this);
    }

    void checkNumberOperand(const Token& op, const Value& operand) {
        if (std::holds_alternative<double>(operand)) return;
        throw RuntimeError(op, "Operand must be a number.");
    }

    void checkNumberOperands(const Token& op, const Value& left, const Value& right) {
        if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) return;
        throw RuntimeError(op, "Operands must be numbers.");
    }

    std::string stringify(const Value& value) {
        return std::visit([](auto&& v) -> std::string {
            using T = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<T, std::monostate>) {
                return "nil";
            } else if constexpr (std::is_same_v<T, double>) {
                std::string text = std::to_string(v);
                // strip trailing zeros: "1.500000" -> "1.5", "2.000000" -> "2"
                text.erase(text.find_last_not_of('0') + 1, std::string::npos);
                if (!text.empty() && text.back() == '.') text.pop_back();
                return text;
            } else if constexpr (std::is_same_v<T, bool>) {
                return v ? "true" : "false";
            } else {
                return v;   // std::string — return as-is
            }
        }, value);
    }

public:
    // public entry point

    void interpret(Expr& expression) {
        try {
            Value value = evaluate(expression);
            std::cout << stringify(value) << "\n";
        } catch (const RuntimeError& error) {
            // Propagate to the top-level handler in Estus.cpp
            throw;
        }
    }

    // visitor implementations

    Value visitLiteral(Literal& literal) override {
        return literal.m_value;
    }

    Value visitGrouping(Grouping& grouping) override {
        return evaluate(*grouping.m_expr);
    }

    Value visitUnary(Unary& unary) override {
        Value right = evaluate(*unary.m_right);

        switch (unary.m_op.m_type) {
            case TokenType::MINUS:
                checkNumberOperand(unary.m_op, right);
                return -std::get<double>(right);

            case TokenType::BANG:
                if (std::holds_alternative<bool>(right))
                    return !std::get<bool>(right);
                if (std::holds_alternative<std::monostate>(right))
                    return true;   // !nil == true
                return false;      // numbers/strings are truthy

            default:
                throw RuntimeError(unary.m_op, "Unknown unary operator.");
        }
    }

    Value visitBinary(Binary& binary) override {
        Value left  = evaluate(*binary.m_left);
        Value right = evaluate(*binary.m_right);

        switch (binary.m_op.m_type) {
            // Arithmetic
            case TokenType::MINUS:
                checkNumberOperands(binary.m_op, left, right);
                return std::get<double>(left) - std::get<double>(right);
            case TokenType::STAR:
                checkNumberOperands(binary.m_op, left, right);
                return std::get<double>(left) * std::get<double>(right);
            case TokenType::SLASH:
                checkNumberOperands(binary.m_op, left, right);
                return std::get<double>(left) / std::get<double>(right);
            case TokenType::PLUS:
                if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right))
                    return std::get<double>(left) + std::get<double>(right);
                if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right))
                    return std::get<std::string>(left) + std::get<std::string>(right);
                throw RuntimeError(binary.m_op, "Operands must be two numbers or two strings.");

            // Comparison
            case TokenType::GREATER:
                checkNumberOperands(binary.m_op, left, right);
                return std::get<double>(left) > std::get<double>(right);
            case TokenType::GREATER_EQUAL:
                checkNumberOperands(binary.m_op, left, right);
                return std::get<double>(left) >= std::get<double>(right);
            case TokenType::LESS:
                checkNumberOperands(binary.m_op, left, right);
                return std::get<double>(left) < std::get<double>(right);
            case TokenType::LESS_EQUAL:
                checkNumberOperands(binary.m_op, left, right);
                return std::get<double>(left) <= std::get<double>(right);

            // Equality — works across any Value type
            case TokenType::EQUAL_EQUAL:
                return left == right;
            case TokenType::BANG_EQUAL:
                return left != right;

            default:
                throw RuntimeError(binary.m_op, "Unknown binary operator.");
        }
    }
};
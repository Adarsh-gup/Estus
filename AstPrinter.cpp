#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <variant>
#include "Expressions/expr.hpp"

// ─────────────────────────────────────────────
//  AstPrinter — produces a string from nodes
// ─────────────────────────────────────────────
class AstPrinter : public ExprVisitor {
public:
    // Entry point — returns a plain std::string by unwrapping the Value
    std::string print(Expr& expr) {
        return std::get<std::string>(expr.accept(*this));
    }

    Value visitBinary(Binary& binary) override {
        return parenthesize(binary.m_op.getLexeme(),
                            { binary.m_left.get(), binary.m_right.get() });
    }

    Value visitGrouping(Grouping& group) override {
        return parenthesize("group", { group.m_expr.get() });
    }

    Value visitLiteral(Literal& literal) override {
        // std::monostate represents null / nil
        return std::visit([](auto&& val) -> Value {
            using T = std::decay_t<decltype(val)>;
            if constexpr (std::is_same_v<T, std::monostate>) {
                return std::string("nil");
            } else if constexpr (std::is_same_v<T, double>) {
                std::ostringstream oss;
                oss << val;          // avoids trailing zeros from std::to_string
                return oss.str();
            } else if constexpr (std::is_same_v<T, bool>) {
                return std::string(val ? "true" : "false");
            } else {
                return val;          // already a std::string
            }
        }, literal.m_value);
    }

    Value visitUnary(Unary& unary) override {
        return parenthesize(unary.m_op.getLexeme(), { unary.m_right.get() });
    }

private:
    // Wraps an operator name and a list of sub-expressions in parentheses.
    // Each child's accept() returns a Value; we extract the string from it.
    // "operator" is a reserved keyword in C++, so we use "op" instead.
    Value parenthesize(const std::string& op,
                       std::initializer_list<Expr*> exprs) {
        std::string result = "(" + op;
        for (Expr* expr : exprs) {
            result += " ";
            result += std::get<std::string>(expr->accept(*this));
        }
        result += ")";
        return result;
    }
};

/* 
int main() {
    // Build the expression tree using unique_ptr (no raw new/delete)
    auto expression = std::make_unique<Binary>(
        std::make_unique<Unary>(
            Token(TokenType::MINUS, "-", std::monostate{}, 1),
            std::make_unique<Literal>(123.0)
        ),
        Token(TokenType::STAR, "*", std::monostate{}, 1),
        std::make_unique<Grouping>(
            std::make_unique<Literal>(45.67)
        )
    );

    // Expected output: (* (- 123) (group 45.67))
    std::cout << AstPrinter().print(*expression) << "\n";
    return 0;
} 
*/

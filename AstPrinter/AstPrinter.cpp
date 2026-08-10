#include "AstPrinter.hpp"
#include <sstream>

// Public entry point

std::string AstPrinter::print(Expr& expr) {
    return std::get<std::string>(expr.accept(*this));
}

// Visitor implementations 

Value AstPrinter::visitBinary(Binary& binary) {
    return parenthesize(binary.m_op.getLexeme(),
                        { binary.m_left.get(), binary.m_right.get() });
}

Value AstPrinter::visitGrouping(Grouping& grouping) {
    return parenthesize("group", { grouping.m_expr.get() });
}

Value AstPrinter::visitLiteral(Literal& literal) {
    // std::monostate represents nil
    return std::visit([](auto&& val) -> Value {
        using T = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<T, std::monostate>) {
            return std::string("nil");
        } else if constexpr (std::is_same_v<T, double>) {
            std::ostringstream oss;
            oss << val;   // avoids trailing zeros from std::to_string
            return oss.str();
        } else if constexpr (std::is_same_v<T, bool>) {
            return std::string(val ? "true" : "false");
        } else {
            return val;   // already a std::string
        }
    }, literal.m_value);
}

Value AstPrinter::visitUnary(Unary& unary) {
    return parenthesize(unary.m_op.getLexeme(), { unary.m_right.get() });
}

// Private helper 

Value AstPrinter::parenthesize(const std::string& op,
                                std::initializer_list<Expr*> exprs) {
    std::string result = "(" + op;
    for (Expr* expr : exprs) {
        result += " ";
        result += std::get<std::string>(expr->accept(*this));
    }
    result += ")";
    return result;
}

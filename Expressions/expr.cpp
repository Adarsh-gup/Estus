#include <iostream>


class Expr {
public:
virtual ~Expr = default;
};
class Binary : public Expr {
public:
    const std::unique_ptr<Expr> m_left;
    const Token m_op;
    const std::unique_ptr<Expr> m_right;

    Binary(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right)
        : m_left(std::move(left)), m_op(std::move(op)), m_right(std::move(right)) {}
};
class Grouping : public Expr {
public:
    const std::unique_ptr<Expr> m_expr;

    Grouping(std::unique_ptr<Expr> expr) : m_expr(std::move(expr)) {}
};
class Literal : public Expr {
public:
    const std::variant<std::monostate, double, std::string> m_value;

    Literal(std::variant<std::monostate, double, std::string> value)
        : m_value(std::move(value)) {}
};
class Unary : public Expr {
public:
    const Token m_op;
    const std::unique_ptr<Expr> m_right;

    Unary(Token op, std::unique_ptr<Expr> right)
        : m_op(std::move(op)), m_right(std::move(right)) {}
};

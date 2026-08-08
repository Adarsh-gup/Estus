#include <vector>
#include "../Expressions/expr.hpp"
#include "../Token/Token.hpp"

class Parser{
private: 
std::vector<Token> m_tokens{};
int m_current {0};

Token peek() {
    return m_tokens[m_current];
}
boolean isAtEnd() {
    return peek().m_type == TokenType::END_OF_FILE;
  }
bool check(TokenType type){
    if(!isAtEnd()) return false;
    return peek().m_type == type;

}
Token previous(){
    return m.tokens[m_current-1];
}
Token advance() {
    if (!isAtEnd()) current++;
    return previous();
  }
bool match(std::initializer_list<TokenType> types){
    for (const auto& type: types){
        if(check(type)){
            advance();
            return true;
        }
    }
    return false;
}
Token consume(TokenType type, String message) {
    if (check(type)) return advance();

  }


Expr expresion() {
return equality();
}
Expr equality() {
Expr expr = comparison();
while (match(Tokentype::BANG_EQUAL, Tokentype::EQUAL_EQUAL)){
      Token operetor = previous();
      Expr right = comparison();
      expr = new Expr.Binary(expr, operator, right);
    
}
return expr;
}
Expr comparison(){
    Expr expr = term();
    while (match(Tokentype::GREATER, Tokentype::GREATER_EQUAL,
         Tokentype::LESS, Tokentype::LESS_EQUAL))
    {
        Token operator = previous();
        Expr right = term();
        expr = new Expr.Binary(expr, operator, right);
    }
    return expr;
}
Expr term() {
    Expr expr = factor();

    while (match(Tokentype::MINUS, Tokentype::PLUS)) {
      Token operator = previous();
      Expr right = factor();
      expr = new Expr.Binary(expr, operator, right);
    }

    return expr;
  }
Expr factor() {
    Expr expr = unary();

    while (match(Tokentype::SLASH, Tokentype::STAR)) {
      Token operator = previous();
      Expr right = unary();
      expr = new Expr.Binary(expr, operator, right);
    }

    return expr;
  }
Expr unary(){
    if (match(Tokentype::BANG, Tokentype::MINUS)){
        Token operator = previous();
        Expr right = unary();
        return new Expr.Unary(operator , right );
    }
    return primary();
}
Expr primary() {
    if (match(Tokentype::FALSE)) return new Expr.Literal(false);
    if (match(Tokentype::TRUE)) return new Expr.Literal(true);
    if (match(Tokentype::NIL)) return new Expr.Literal(null);

    if (match(Tokentype::NUMBER, Tokentype::STRING)) {
      return new Expr.Literal(previous().literal);
    }

    if (match(Tokentype::LEFT_PAREN)) {
      Expr expr = expression();
      consume(Tokentype::RIGHT_PAREN, "Expect ')' after expression.");
      return new Expr.Grouping(expr);
    }
  }

public:
Parser(std::vector<Token> tokens) : m_tokens{tokens} {}

};
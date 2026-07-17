#include "Scanner.hpp"
#include "../error.hpp"
bool Scanner::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
            c == '_';
}

bool Scanner::isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}
bool Scanner::isDigit(char c) {
    return c >= '0' && c <= '9';
}
bool Scanner::EndofSource() {
    return m_current >= static_cast<int>(m_source.length());
}

char Scanner::advance() {
    return m_source[m_current++];
}

void Scanner::addToken(TokenType type) {
    addToken(type, std::monostate{});
}

void Scanner::addToken(TokenType type, std::variant<std::monostate, double, std::string> literal) {
    std::string text { m_source.substr(m_start, m_current - m_start) };
    m_tokens.emplace_back(Token{type, text, literal, m_line});
}

bool Scanner::match(char expected) {
    if (EndofSource()) return false;
    if (expected != m_source[m_current]) return false;
    m_current++;
    return true;
}

char Scanner::peek() {
    if (EndofSource()) return '\0';
    return m_source[m_current];
}

char Scanner::peekNext() {
    if (m_current + 1 >= static_cast<int>(m_source.length())) return '\0';
    return m_source[m_current + 1];
}

void Scanner::string() {
    while (peek() != '"' && !EndofSource()) {
        if (peek() == '\n') m_line++;
        advance();
    }
    if (EndofSource()) {
        error(m_line, "Unterminated string.");
        return;
    }
    advance(); // closing "
    std::string literal_value = m_source.substr(m_start + 1, m_current - m_start - 2);
    addToken(TokenType::STRING, literal_value);
}

void Scanner::numbers() {
    while (isDigit(peek())) advance();

    // Look for a fractional part.
    if (peek() == '.' && isDigit(peekNext())) {
        // Consume the "."
        advance();
        while (isDigit(peek())) advance();
    }
    double literal_value = std::stod(m_source.substr(m_start, m_current - m_start));
    addToken(TokenType::NUMBER, literal_value);
}
void Scanner::identifier() {
    while (isAlphaNumeric(peek())) advance();
    std::string text = m_source.substr(m_start, m_current - m_start);
    auto it = keywords.find(text);
    TokenType type = (it == keywords.end()) ? TokenType::IDENTIFIER : it->second;
    addToken(type);
}

void Scanner::scanToken() {
    char c = advance();
    switch (c) {
        case '(': addToken(TokenType::LEFT_PAREN);  break;
        case ')': addToken(TokenType::RIGHT_PAREN); break;
        case '{': addToken(TokenType::LEFT_BRACE);  break;
        case '}': addToken(TokenType::RIGHT_BRACE); break;
        case ',': addToken(TokenType::COMMA);       break;
        case '.': addToken(TokenType::DOT);         break;
        case '-': addToken(TokenType::MINUS);       break;
        case '+': addToken(TokenType::PLUS);        break;
        case ';': addToken(TokenType::SEMICOLON);   break;
        case '*': addToken(TokenType::STAR);        break;
        case '!':
            addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
            break;
        case '=':
            addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
            break;
        case '<':
            addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
            break;
        case '>':
            addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
            break;
        case '/':
            if (match('/')) {
                while (peek() != '\n' && !EndofSource()) advance();
            }
            else if (match('*')){
                int depth = 1;
                while ( depth != 0 && !EndofSource() ) {
                 if (peek() == '/' && peekNext() == '*'){
                    ++depth;
                    advance(); 
                    advance();
                }
                 else if ( peek() == '*' && peekNext() == '/') {
                    --depth;
                    advance(); 
                    advance();
                }
                 else if (peek() == '\n') { m_line++; advance();}
                 else {advance();}
               }
               if (EndofSource()) { error(m_line, "Unterminated comment.");}
            
            }
            else {
                addToken(TokenType::SLASH);
            }
            break;
        case ' ':
        case '\r':
        case '\t':
            // Ignore whitespace.
            break;
        case '\n':
            m_line++;
            break;
        case '"':
            string();
            break;
        default:
            if (isDigit(c)) {
                numbers();
            }
            else if (isAlpha(c)){
                identifier();
            }
            else {
                error(m_line, "Unexpected character.");
            }
            break;
    }
}

std::vector<Token> Scanner::scanTokens() {
    while (!EndofSource()) {
        m_start = m_current;
        scanToken();
    }
    m_tokens.emplace_back(Token{TokenType::END_OF_FILE, "", std::monostate{}, m_line});
    return m_tokens;
}


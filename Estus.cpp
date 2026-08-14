#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Parser/Parser.hpp"
#include "Statement/Statement.hpp"
#include "Scanner/Scanner.hpp"
#include "error.hpp"
#include "RuntimeError/RuntimeError.hpp"
#include "Interpreter/interpreter.hpp"

bool hadError = false;
bool hadRuntimeError = false;

namespace PrintError {
 void error(int line, const std::string& message) {
    report(line, "", message);
}
void error(const Token& token, const std::string& message) {
    if (token.m_type == TokenType::END_OF_FILE) {
      report(token.m_line, " at end", message);
    } else {
      report(token.m_line, " at '" + token.m_lexeme + "'", message);
    }
  }
}
void runtimeError(const RuntimeError& error) {
    std::cerr << error.what() << "\n[line " << error.m_token.m_line << "]\n";
    hadRuntimeError = true;
}

void report(int line, const std::string& where, const std::string& message) {
    std::cout << "[line " << line << "] Error" << where + ": " << message << "\n";
    hadError = true;
}

void run(const std::string& source) {
    Scanner scan {source};
    std::vector<Token> tokens { scan.scanTokens() };
    Parser parser(tokens);
    std::vector<std::unique_ptr<Stmt>> stmt = parser.parse();
    if (hadError) return;

    static Interpreter interpreter;   // value, not pointer
    try {
        interpreter.interpret(stmt);   
    } catch (const RuntimeError& error) {
        runtimeError(error);
    }
}
void runFile ( const char* path) {
    std::ifstream file(path);
    if (!file){
        std::cout << "Error could not open" << path << std::endl;
        std::exit(74);
    }
/*
    I won't use the streambuf iterator as described below,

    std::string content { std::istreambuf_iterator<char>(file), std::istreambuf_iterator() };

    because the compile will try
    
    initializer_list<char> constructor first and iterator ≠ char 
*/
    std::istreambuf_iterator<char> begin(file), end;
    std::string content(begin, end);
    run(content);
    if (hadError)        std::exit(65);
    if (hadRuntimeError) std::exit(70);
}
void runPrompt() {
    for (;;) {
        std::cout << "> ";
        std::string line;
        if (!std::getline(std::cin, line)) break;
        run(line);
        hadError = false;
        hadRuntimeError = false;   // reset between REPL lines
    }
}
int main( int argc, char* argv[]){
    if (argc > 2) {
    std::cout << "Usage: jlox [script]" << std::endl;
        return 64;
    }
    else if (argc == 2) {
        runFile(argv[1]);
    }
    else {
        runPrompt();
    }
    return 0;
}

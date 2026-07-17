#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "error.hpp"
#include "Scanner/Scanner.hpp"

bool hadError = false;
void error(int line, std::string message) {
    report(line, "", message);
}

void report(int line, std::string where, std::string message) {
    std::cout << "[line " << line << "] Error" << where + ": " << message << "\n";
    hadError = true;
}

void run(const std::string& source) {
    Scanner scan {source};
    std::vector<Token> token { scan.scanTokens()};
    for (const auto& t : token) {
        std::cout << t << "\n";
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
    if(hadError) std::exit(65);
}
void runPrompt() {

    for(;;) {
    std::cout << "> ";
    std::string line;
    if (!std::getline(std::cin, line)) {break;}
    run(line);
    hadError = false;
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

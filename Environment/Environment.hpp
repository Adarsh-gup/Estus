#pragma once
#include <string>
#include <unordered_map>
#include "../Expressions/expr.hpp"
#include "../Token/Token.hpp"
#include "../RuntimeError/RuntimeError.hpp"

class Environment {
private:
    std::unordered_map<std::string, Value> m_values{};

public:
    void define(const std::string& name, const Value& value) {
        m_values[name] = value;
    }

    Value get(const Token& name) {
        auto it = m_values.find(name.m_lexeme);
        if (it != m_values.end()) {
            return it->second;
        }
        throw RuntimeError(name, "Undefined variable '" + name.m_lexeme + "'.");
    }
};

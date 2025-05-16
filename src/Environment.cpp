//
// Created by shesh on 5/15/2025.
//


#include "../include/Environment.h"

#include "../include/RuntimeError.h"

namespace lox {
    void Environment::define(const std::string &name, const TokenLiteral &value) {
        values[name] = value;
    }

    TokenLiteral Environment::get(Token name) {
        if (values.contains(name.getLexeme())) {
            return values.at(name.getLexeme());
        }

        throw RuntimeError(name, "Undefine variable '" + name.getLexeme() + "'.");
    }
}

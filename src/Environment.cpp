//
// Created by shesh on 5/15/2025.
//


#include "../include/Environment.h"

#include "../include/RuntimeError.h"

namespace lox {
    Environment::Environment() : enclosing(nullptr) {
    };

    Environment::Environment(std::shared_ptr<Environment> enclosing) : enclosing(std::move(enclosing)) {
    }


    void Environment::define(const std::string &name, const TokenLiteral &value) {
        values[name] = value;
    }

    TokenLiteral Environment::get(Token name) {
        if (values.contains(name.getLexeme())) {
            return values.at(name.getLexeme());
        }

        if (enclosing != nullptr) return enclosing->get(name);

        throw RuntimeError(name, "Undefine variable '" + name.getLexeme() + "'.");
    }

    void Environment::assign(Token name, TokenLiteral &value) {
        if (values.contains(name.getLexeme())) {
            values[name.getLexeme()] = value;
            return;
        }

        if (enclosing != nullptr) {
            enclosing->assign(name, value);
            return;
        }

        throw RuntimeError(name, "Undefine variable '" + name.getLexeme() + "'.");
    }
}

//
// Created by shesh on 5/15/2025.
//

#pragma once
#include <memory>
#include <unordered_map>

#include "Token.h"

namespace lox {
    class Environment {
        std::shared_ptr<Environment> enclosing;
        std::unordered_map<std::string, TokenLiteral> values;

    public:
        Environment();

        Environment(std::shared_ptr<Environment> enclosing);

        void define(const std::string &name, const TokenLiteral &value);

        TokenLiteral get(Token name);

        void assign(Token name, TokenLiteral &value);
    };
}

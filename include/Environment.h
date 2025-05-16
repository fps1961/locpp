//
// Created by shesh on 5/15/2025.
//

#pragma once
#include <unordered_map>

#include "Token.h"

namespace lox {
    class Environment {
        std::unordered_map<std::string, TokenLiteral> values;

    public:
        void define(const std::string &name, const TokenLiteral &value);

        TokenLiteral get(Token name);
    };
}

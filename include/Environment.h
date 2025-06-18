//
// Created by shesh on 5/15/2025.
//

#pragma once
#include <memory>
#include <unordered_map>

#include "Token.h"

namespace lox {
    class Environment : public std::enable_shared_from_this<Environment> {
        std::shared_ptr<Environment> enclosing;
        std::unordered_map<std::string, TokenLiteral> values;

    public:
        Environment();

        Environment(std::shared_ptr<Environment> enclosing);

        void define(const std::string &name, const TokenLiteral &value);

        TokenLiteral getAt(int distance, const std::string &name);

        std::shared_ptr<Environment> ancestor(int distance);

        TokenLiteral get(Token name);

        void assign(Token name, TokenLiteral &value);
    };
}

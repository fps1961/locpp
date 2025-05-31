//
// Created by shesh on 5/30/2025.
//

#pragma once
#include "Interpreter.h"
#include "Token.h"

namespace lox {
    class LoxCallable {
    public:
        virtual TokenLiteral call(Interpreter &interpreter, std::vector<TokenLiteral> arguments) = 0;

        virtual std::string toString() = 0;

        virtual int arity() = 0;
    };
}

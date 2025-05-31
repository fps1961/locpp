//
// Created by shesh on 5/30/2025.
//

#pragma once
#include <vector>
#include "TokenLiteral.h"

namespace lox {
    class Interpreter;

    class LoxCallable {
    public:
        virtual TokenLiteral call(Interpreter &interpreter, std::vector<TokenLiteral> arguments) = 0;

        virtual std::string toString() = 0;

        virtual int arity() = 0;

        virtual TokenLiteral call() = 0;
    };
}

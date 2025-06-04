//
// Created by shesh on 6/3/2025.
//

#pragma once
#include "LoxCallable.h"


namespace lox {
    class Function;

    class LoxFunction : public LoxCallable {
        std::shared_ptr<Function> declaration;

    public:
        explicit LoxFunction(std::shared_ptr<Function> declaration);

        TokenLiteral call() override;

        TokenLiteral call(Interpreter &interpreter, std::vector<TokenLiteral> &arguments) override;

        int arity() override;

        std::string toString() override;
    };
}

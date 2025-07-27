//
// Created by shesh on 6/3/2025.
//

#pragma once
#include "LoxCallable.h"


namespace lox {
    class Environment;
}

namespace lox {
    class Function;

    class LoxFunction : public LoxCallable {
        std::shared_ptr<Function> declaration;
        std::shared_ptr<Environment> closure;

    public:
        explicit LoxFunction(std::shared_ptr<Function> declaration,
                             std::shared_ptr<Environment> environment) : declaration(std::move(declaration)),
                                                                         closure(std::move(environment)) {
        };

        std::shared_ptr<LoxFunction> bind(const std::shared_ptr<LoxInstance>& loxInstance);

        TokenLiteral call() override;

        TokenLiteral call(Interpreter &interpreter, std::vector<TokenLiteral> &arguments) override;

        int arity() override;

        std::string toString() override;
    };
}

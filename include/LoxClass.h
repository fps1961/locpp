#pragma once
#include <string>
#include "LoxCallable.h"
#include "TokenLiteral.h"

namespace lox {
    class LoxClass : public LoxCallable,
                     public std::enable_shared_from_this<LoxClass> {
        std::string name;

    public:
        explicit LoxClass(std::string name) : name(std::move(name)) {
        }

        std::string toString() override;

        TokenLiteral call(Interpreter&, std::vector<TokenLiteral>&) override;

        TokenLiteral call() override { return {}; }

        int arity() override;
    };
}

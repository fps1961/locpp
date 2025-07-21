#pragma once
#include <memory>
#include <unordered_map>

#include "TokenLiteral.h"


namespace lox {
    class Token;
    class LoxClass;

    class LoxInstance {
        std::shared_ptr<lox::LoxClass> loxClass;

        std::unordered_map<std::string, TokenLiteral> fields;

    public:
        explicit LoxInstance(const std::shared_ptr<LoxClass> &loxClass) : loxClass(loxClass) {
        };

        TokenLiteral get(const Token &name);

        void set(const ::lox::Token &name, const TokenLiteral &value);

        std::string toString() const;
    };
}

//
// Created by shesh on 7/13/2025.
//

#include "../include/LoxInstance.h"

#include <variant>
#include <variant>
#include <variant>
#include <variant>

#include "../include/LoxClass.h"
#include "../include/RuntimeError.h"
#include "../include/Token.h"

namespace lox {
    std::string LoxInstance::toString() const {
        return loxClass->toString() + " instance";
    }

    TokenLiteral LoxInstance::get(const Token &name) {
        if (fields.contains(name.getLexeme())) {
            return fields[name.getLexeme()];
        }

        if (const auto &method = loxClass->findMethod(name.getLexeme());
            method != nullptr) {
            return method->bind(shared_from_this());
        };

        throw new RuntimeError(name, "Undefined property '" + name.getLexeme() + "'.");
    }

    void LoxInstance::set(const Token &name, const TokenLiteral &value) {
        fields[name.getLexeme()] = value;
    }
}

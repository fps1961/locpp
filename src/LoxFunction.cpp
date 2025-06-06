//
// Created by shesh on 6/3/2025.
//

#include "../include/LoxFunction.h"

#include <variant>
#include <vector>

#include "../include/Environment.h"
#include "../include/Interpreter.h"
#include "../include/LoxReturn.h"

namespace lox {
    TokenLiteral LoxFunction::call() {
        return {};
    }

    TokenLiteral LoxFunction::call(Interpreter &interpreter, std::vector<TokenLiteral> &arguments) {
        const auto environment = std::make_shared<Environment>(closure);
        for (size_t i = 0; i < declaration->getParams().size(); ++i) {
            environment->define(declaration->getParams()[i].getLexeme(), arguments[i]);
        }

        try {
            interpreter.executeBlock(declaration->getBody(), environment);
        } catch (LoxReturn &returnValue) {
            return returnValue.value;
        }
        return {};
    }

    int LoxFunction::arity() {
        return declaration->getParams().size();
    }

    std::string LoxFunction::toString() {
        return "<fn" + declaration->getName().getLexeme() + ">";
    }
}

//
// Created by shesh on 7/13/2025.
//

#include "../include/Interpreter.h"
#include "../include/LoxClass.h"

#include "../include/LoxInstance.h"
#include "../include/TokenLiteral.h"

namespace lox {
    LoxClass::LoxClass(std::string name,
                       std::unordered_map<std::string, std::shared_ptr<LoxFunction> > &methods): name(std::move(name)),
        methods(std::move(methods)) {
    };

    std::shared_ptr<LoxFunction> LoxClass::findMethod(std::string name) {
        if (methods.contains(name)) {
            return methods[name];
        }
        return nullptr;
    }


    TokenLiteral LoxClass::call(Interpreter &, std::vector<TokenLiteral> &) {
        auto loxInstance = std::make_shared<LoxInstance>(shared_from_this());
        return loxInstance;
    }

    std::string LoxClass::toString() {
        return name;
    }

    int LoxClass::arity() { return 0; }
}

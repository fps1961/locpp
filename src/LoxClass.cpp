//
// Created by shesh on 7/13/2025.
//

#include "../include/Interpreter.h"
#include "../include/LoxClass.h"

#include "../include/LoxInstance.h"
#include "../include/TokenLiteral.h"

namespace lox {


    TokenLiteral LoxClass::call(Interpreter&, std::vector<TokenLiteral>&) {
        auto loxInstance = std::make_shared<LoxInstance>(shared_from_this());
        return loxInstance;
    }

    std::string LoxClass::toString() {
        return name;
    }

    int LoxClass::arity() {return 0;}

}
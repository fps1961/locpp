//
// Created by shesh on 7/13/2025.
//

#include "../include/LoxInstance.h"
#include "../include/LoxClass.h"

namespace lox {
    std::string LoxInstance::toString() const {
        return loxClass->toString() + " instance";
    }

}
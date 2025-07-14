#pragma once
#include <memory>


namespace lox {
    class LoxClass;

    class LoxInstance {
        std::shared_ptr<lox::LoxClass> loxClass;

    public:
        LoxInstance(const std::shared_ptr<LoxClass> &loxClass) : loxClass(loxClass) {
        };

        std::string toString() const;
    };
}

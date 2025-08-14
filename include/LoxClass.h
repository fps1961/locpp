#pragma once
#include <string>
#include "LoxCallable.h"
#include "TokenLiteral.h"

namespace lox {
    class LoxFunction;
}

namespace lox {
    class LoxClass : public LoxCallable,
                     public std::enable_shared_from_this<LoxClass> {
        std::string name;
        std::shared_ptr<LoxClass>& superClass;
        std::unordered_map<std::string, std::shared_ptr<LoxFunction> > methods;

    public:
        explicit LoxClass(std::string name,
                          std::shared_ptr<LoxClass>& superClass,
                          std::unordered_map<std::string, std::shared_ptr<LoxFunction> > &methods);

        std::shared_ptr<LoxFunction> findMethod(std::string aName);

        std::string toString() override;

        TokenLiteral call(Interpreter &, std::vector<TokenLiteral> &) override;

        TokenLiteral call() override { return {}; }

        int arity() override;
    };
}

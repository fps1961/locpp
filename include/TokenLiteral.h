//
// Created by shesh on 6/3/2025.
//

#pragma once
#include <memory>
#include <variant>
#include <string>

namespace lox {
    class LoxCallable; // forward declaration

    using TokenLiteral = std::variant<
        std::monostate,
        std::string,
        double,
        bool,
        std::shared_ptr<LoxCallable>
    >;
}

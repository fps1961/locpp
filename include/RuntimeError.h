//
// Created by shesh on 5/9/2025.
//

#pragma once
#include <stdexcept>

#include "Token.h"

namespace lox {
    class RuntimeError final : public std::runtime_error {
    public:
        const Token &token;

        RuntimeError(const Token &token, const std::string_view message) : std::runtime_error{message.data()}, token{token} {
        };
    };
}

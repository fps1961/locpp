//
// Created by shesh on 3/7/2025.
//

#pragma once
#include <string>
#include <variant>

#include "Token.h"
#include "TokenType.h"

namespace lox {
    using Literal = std::variant<std::monostate, std::string, double, bool>;

    class Token {
        TokenType type;
        std::string lexeme;
        Literal literal;
        int line;

    public:
        Token(TokenType type, std::string lexeme, Literal literal, int line);

        [[nodiscard]] std::string toString() const;

        friend std::ostream &operator<<(std::ostream &os, const Token &token);
    };
}

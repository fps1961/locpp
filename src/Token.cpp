//
// Created by shesh on 3/7/2025.
//


#include <utility>

#include "../include/Token.h"

#include <sstream>

namespace lox {
    Token::Token(const lox::TokenType type, std::string lexeme, Literal literal, const int line) : type(type),
        lexeme(std::move(lexeme)),
        literal(std::move(literal)), line(line) {
    }

    std::string Token::toString() const {
        std::ostringstream oss;
        oss << tokenTypeToString(type) << " " << lexeme << " ";

        std::visit([&oss]<typename T>(T &&arg) {
            using T = std::decay_t<T>;
            if constexpr (std::is_same_v<T, std::monostate>) {
                oss << "null";
            } else if constexpr (std::is_same_v<std::decay_t<T>, std::string> ||
                                 std::is_same_v<std::decay_t<T>, double>) {
                oss << arg;
            } else if constexpr (std::is_same_v<T, bool>) {
                oss << (arg ? "true" : "false");
            }
        }, literal);

        return oss.str();
    }


    std::ostream &operator<<(std::ostream &os, const Token &token) {
        return os << token.toString();
    }
}

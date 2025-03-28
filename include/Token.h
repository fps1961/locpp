//
// Created by sheshan on 3/7/2025.
//

#pragma once
#include <string>
#include <variant>

#include "Token.h"
#include "TokenType.h"

namespace lox
{
    using TokenLiteral = std::variant<std::monostate, std::string, double, bool>;

    struct TokenLiteralEvaluator
    {
        std::string operator()(std::monostate) const { return "nil"; }
        std::string operator()(const std::string& s) const { return s; }
        std::string operator()(const double d) const { return std::to_string(d); }
        std::string operator()(const bool b) const { return b ? "true" : "false"; }
    };

    class Token
    {
        TokenType type;
        std::string lexeme;
        TokenLiteral literal;
        int line;

    public:
        Token(TokenType type, std::string lexeme, TokenLiteral literal, int line);

        [[nodiscard]] const std::string& getLexeme() const;

        [[nodiscard]] const TokenType& getTokenType() const;

        [[nodiscard]] const TokenLiteral& getLiteral() const;

        [[nodiscard]] int getLine() const;

        [[nodiscard]] std::string toString() const;

        friend std::ostream& operator<<(std::ostream& os, const Token& token);
    };
}

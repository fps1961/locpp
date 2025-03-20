//
// Created by shesh on 3/18/2025.
//

#pragma once
#include <assert.h>
#include <vector>

#include "Expr.h"
#include "Token.h"

namespace lox {
    class Parser {
    public:
        explicit Parser(const std::vector<Token> &tokens);

        std::shared_ptr<Expr> parse();

    private:
        class ParseError final : public std::runtime_error {
        public:
            ParseError() : std::runtime_error("Parse Error") {
            };

            explicit ParseError(const std::string &message) : std::runtime_error(message) {
            };
        };

        const std::vector<Token> &tokens{};
        int current{0};

        std::shared_ptr<Expr> expression();

        std::shared_ptr<Expr> equality();

        std::shared_ptr<Expr> comparison();

        std::shared_ptr<Expr> term();

        std::shared_ptr<Expr> factor();

        std::shared_ptr<Expr> unary();

        std::shared_ptr<Expr> primary();

        template<typename... T>
        bool match(T... type) {
            assert((... && std::is_same_v<T, TokenType>));

            if ((... || check(type))) {
                advance();
                return true;
            }
            return false;
        }

        Token consume(TokenType tokenType, std::string message);

        bool check(TokenType tokenType) const;

        Token advance();

        bool isAtEnd() const;

        Token peek() const;

        Token previous() const;

        ParseError error(const Token &token, const std::string &message);

        void synchronize();
    };
}

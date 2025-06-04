//
// Created by sheshan on 3/18/2025.
//

#pragma once
#include <cassert>
#include <vector>

#include "Expr.h"
#include "Stmt.h"
#include "Token.h"

namespace lox {
    class Parser {
    public:
        explicit Parser(const std::vector<Token> &tokens);

        std::vector<std::shared_ptr<Stmt> > parse();

    private:
        class ParseError final : public std::runtime_error {
        public:
            ParseError() : std::runtime_error("Parse Error") {
            }

            explicit ParseError(const std::string &message) : std::runtime_error(message) {
            }
        };

        const std::vector<Token> &tokens{};
        int current{0};

        std::shared_ptr<Expr> expression();

        std::shared_ptr<Stmt> declaration();

        std::shared_ptr<Stmt> statement();

        std::shared_ptr<Stmt> forStatement();

        std::shared_ptr<Stmt> ifStatement();

        std::shared_ptr<Stmt> printStatement();

        std::shared_ptr<Stmt> varDeclaration();

        std::shared_ptr<Stmt> whileStatement();

        std::shared_ptr<Stmt> expressionStatement();

        std::shared_ptr<Stmt> function(const std::string &kind);

        std::vector<std::shared_ptr<Stmt> > block();

        std::shared_ptr<Expr> assignment();

        std::shared_ptr<Expr> orExpression();

        std::shared_ptr<Expr> andExpression();

        std::shared_ptr<Expr> equality();

        std::shared_ptr<Expr> comparison();

        std::shared_ptr<Expr> term();

        std::shared_ptr<Expr> factor();

        std::shared_ptr<Expr> unary();

        std::shared_ptr<Expr> finishCall(std::shared_ptr<Expr> &calle);

        std::shared_ptr<Expr> call();

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

        Token consume(TokenType tokenType, const std::string &message);

        [[nodiscard]] bool check(TokenType tokenType) const;

        Token advance();

        [[nodiscard]] bool isAtEnd() const;

        [[nodiscard]] Token peek() const;

        [[nodiscard]] Token previous() const;

        ParseError error(const Token &token, const std::string &message);

        void synchronize();
    };
}

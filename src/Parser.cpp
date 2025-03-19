//
// Created by shesh on 3/18/2025.
//


#include "../include/Parser.h"

namespace lox {
    Parser::Parser(const std::vector<Token> &tokens) : tokens(tokens) {
    };


    std::shared_ptr<Expr> Parser::expression() {
        return equality();
    }

    std::shared_ptr<Expr> Parser::equality() {
        std::shared_ptr<Expr> left = comparison();

        while (match(BANG_EQUAL, EQUAL_EQUAL)) {
            Token op = previous();
            std::shared_ptr<Expr> right = comparison();
            left = std::make_shared<Binary>(left, op, right);
        }
        return left;
    }

    std::shared_ptr<Expr> Parser::comparison() {
        std::shared_ptr<Expr> left = term();

        while (match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL)) {
            Token op = previous();
            std::shared_ptr<Expr> right = term();
            left = std::make_shared<Binary>(left, op, right);
        }
        return left;
    }

    std::shared_ptr<Expr> Parser::term() {
        std::shared_ptr<Expr> left = factor();

        while (match(MINUS, PLUS)) {
            Token op = previous();
            std::shared_ptr<Expr> right = factor();
            left = std::make_shared<Binary>(left, op, right);
        }
        return left;
    }

    std::shared_ptr<Expr> Parser::factor() {
        std::shared_ptr<Expr> left = unary();

        while (match(SLASH, STAR)) {
            Token op = previous();
            std::shared_ptr<Expr> right = unary();
            left = std::make_shared<Binary>(left, op, right);
        }
        return left;
    }

    std::shared_ptr<Expr> Parser::unary() {
        if (match(BANG, MINUS)) {
            Token op = previous();
            std::shared_ptr<Expr> right = unary();
            return std::make_shared<Unary>(op, right);
        }
        return primary();
    }

    std::shared_ptr<Expr> Parser::primary() {
        if (match(FALSE)) return std::make_shared<Literal>(false);
        if (match(TRUE)) return std::make_shared<Literal>(true);
        if (match(NIL)) return std::make_shared<Literal>(nullptr);

        if (match(NUMBER, STRING)) return std::make_shared<Literal>(previous().getLiteral());

        if (match(LEFT_PAREN)) {
            std::shared_ptr<Expr> expr = expression();
            consume(RIGHT_PAREN, "Expect ')' after expression.");
            return std::make_shared<Grouping>(expr);
        }
    }


    bool Parser::check(const TokenType tokenType) const {
        if (isAtEnd()) return false;
        return peek().getTokenType() == tokenType;
    }

    Token Parser::advance() {
        if (!isAtEnd()) ++current;
        return previous();
    }

    bool Parser::isAtEnd() const {
        return peek().getTokenType() == EOF;
    }

    Token Parser::peek() const {
        return tokens.at(current);
    }

    Token Parser::previous() const {
        return tokens.at(current - 1);
    }
}

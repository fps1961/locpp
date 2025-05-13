//
// Created by sheshan on 3/18/2025.
//


#include "../include/Parser.h"

#include "../include/Lox.h"

namespace lox {
    Parser::Parser(const std::vector<Token> &tokens) : tokens(tokens) {
    };

    std::vector<std::shared_ptr<Stmt> > Parser::parse() {
        std::vector<std::shared_ptr<Stmt> > statements{};
        while (!isAtEnd()) {
            statements.push_back(statement());
        }
        return statements;
    }

    std::shared_ptr<Expr> Parser::expression() {
        return equality();
    }

    std::shared_ptr<Stmt> Parser::statement() {
        if (match(PRINT)) return printStatement();

        return expressionStatement();
    }

    std::shared_ptr<Stmt> Parser::printStatement() {
        const std::shared_ptr<Expr> expr = expression();
        consume(SEMICOLON, "Expect ';' after expression.");
        return std::make_shared<Print>(expr);
    }

    std::shared_ptr<Stmt> Parser::expressionStatement() {
        auto expr = expression();
        consume(SEMICOLON, "Expect ';' after expression.");
        return std::make_shared<Expression>(expr);
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

        throw error(peek(), "Expect expression");
    }


    Token Parser::consume(TokenType tokenType, const std::string &message) {
        if (check(tokenType)) return advance();

        throw error(peek(), message);
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
        return peek().getTokenType() == END_OF_FILE;
    }

    Token Parser::peek() const {
        return tokens.at(current);
    }

    Token Parser::previous() const {
        return tokens.at(current - 1);
    }

    Parser::ParseError Parser::error(const Token &token, const std::string &message) {
        Lox::error(token, message);
        return ParseError{""};
    }

    void Parser::synchronize() {
        advance();

        while (!isAtEnd()) {
            if (previous().getTokenType() == SEMICOLON) return;

            switch (peek().getTokenType()) {
                case CLASS:
                case FUN:
                case VAR:
                case FOR:
                case IF:
                case WHILE:
                case PRINT:
                case RETURN:
                    return;
                default:
                    advance();
            }
        }
    }
}

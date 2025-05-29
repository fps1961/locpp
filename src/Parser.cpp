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
            statements.push_back(declaration());
        }
        return statements;
    }

    std::shared_ptr<Expr> Parser::expression() {
        return assignment();
    }

    std::shared_ptr<Stmt> Parser::declaration() {
        try {
            if (match(VAR)) return varDeclaration();
            return statement();
        } catch (ParseError &error) {
            synchronize();
            return nullptr;
        }
    }


    std::shared_ptr<Stmt> Parser::statement() {
        if (match(IF)) return ifStatement();
        if (match(PRINT)) return printStatement();
        if (match(LEFT_BRACE)) return std::make_shared<Block>(block());

        return expressionStatement();
    }

    std::shared_ptr<Stmt> Parser::ifStatement() {
        consume(LEFT_PAREN, "Expect '(' after 'if'.");
        std::shared_ptr<Expr> condition = expression();
        consume(RIGHT_PAREN, "Expect ')' after if condition.");

        std::shared_ptr<Stmt> thenBranch = statement();
        std::shared_ptr<Stmt> elseBranch = nullptr;
        if (match(ELSE)) { elseBranch = statement(); };
        return std::make_shared<If>(condition, thenBranch, elseBranch);
    }


    std::shared_ptr<Stmt> Parser::printStatement() {
        const std::shared_ptr<Expr> expr = expression();
        consume(SEMICOLON, "Expect ';' after expression.");
        return std::make_shared<Print>(expr);
    }

    std::shared_ptr<Stmt> Parser::varDeclaration() {
        Token name = consume(IDENTIFIER, "Expect variable name.");

        std::shared_ptr<Expr> initializer = nullptr;
        if (match(EQUAL)) initializer = expression();

        consume(SEMICOLON, "Expect ';' after variable declaration.");
        return std::make_shared<Var>(name, initializer);
    }


    std::shared_ptr<Stmt> Parser::expressionStatement() {
        auto expr = expression();
        consume(SEMICOLON, "Expect ';' after expression.");
        return std::make_shared<Expression>(expr);
    }

    std::vector<std::shared_ptr<Stmt> > Parser::block() {
        std::vector<std::shared_ptr<Stmt> > statements{};
        while (!check(RIGHT_BRACE) && !isAtEnd()) {
            statements.push_back(declaration());
        }
        consume(RIGHT_BRACE, "Expect '}' after block.");
        return statements;
    }


    std::shared_ptr<Expr> Parser::assignment() {
        std::shared_ptr<Expr> expr = orExpression();

        if (match(EQUAL)) {
            const Token equals = previous();
            std::shared_ptr<Expr> value = assignment();

            if (const Variable *e = dynamic_cast<Variable *>(expr.get())) {
                Token name = e->getName();
                return std::make_shared<Assign>(name, value);
            }
            error(equals, "Invalid assignment target.");
        }

        return expr;
    }

    std::shared_ptr<Expr> Parser::orExpression() {
        std::shared_ptr<Expr> expr = andExpression();
        while (match(OR)) {
            Token op = previous();
            std::shared_ptr<Expr> right = andExpression();
            expr = std::make_shared<Logical>(expr, op, right);
        }
        return expr;
    }

    std::shared_ptr<Expr> Parser::andExpression() {
        std::shared_ptr<Expr> expr = equality();

        while (match(AND)) {
            Token op = previous();
            std::shared_ptr<Expr> right = equality();
            expr = std::make_shared<Logical>(expr, op, right);
        }
        return expr;
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
        if (match(NIL)) return std::make_shared<Literal>(std::monostate{});

        if (match(NUMBER, STRING)) return std::make_shared<Literal>(previous().getLiteral());

        if (match(IDENTIFIER)) return std::make_shared<Variable>(previous());

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

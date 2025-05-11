//
// Created by sheshan on 3/27/2025.
//
#pragma once

#include "Expr.h"

namespace lox {
    class Interpreter : public ExprVisitor {
    public:
        void interpret(const std::shared_ptr<Expr> &expression);

        TokenLiteral visitGroupingExpr(const Grouping &expr) override;

        TokenLiteral visitLiteralExpr(const Literal &expr) override;

        TokenLiteral visitUnaryExpr(const Unary &expr) override;

        TokenLiteral visitBinaryExpr(const Binary &expr) override;

    private:
        [[nodiscard]] bool isTruthy(TokenLiteral token_literal) const;

        [[nodiscard]] bool isEqual(const TokenLiteral &a, const TokenLiteral &b) const;

        void checkNumberOperand(const Token &token, const TokenLiteral &operand);

        void checkNumberOperands(const Token &token, const TokenLiteral &left, const TokenLiteral &right);

        TokenLiteral evaluate(const std::shared_ptr<Expr> &expr);

        std::string stringify(TokenLiteral &object);
    };
}

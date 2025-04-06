//
// Created by sheshan on 3/27/2025.
//
#pragma once

#include "Expr.h"

namespace lox
{
    class Interpreter : public Visitor
    {
    public:
        TokenLiteral visitGroupingExpr(const Grouping& expr) override;
        TokenLiteral visitLiteralExpr(const Literal& expr) override;
        TokenLiteral visitUnaryExpr(const Unary& expr) override;
        TokenLiteral visitBinaryExpr(const Binary &expr) override;

    private:
        [[nodiscard]] bool isTruthy(TokenLiteral token_literal) const;
        TokenLiteral evaluate(const std::shared_ptr<Expr>& expr);
    };
}

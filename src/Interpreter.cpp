#include "../include/Interpreter.h"
//
// Created by sheshan on 3/27/2025.
//
namespace lox
{
    TokenLiteral Interpreter::visitGroupingExpr(const Grouping& expr)
    {
        return evaluate(expr.getExpression());
    }

    TokenLiteral Interpreter::visitLiteralExpr(const Literal& expr)
    {
        return expr.getValue();
    }

    TokenLiteral Interpreter::visitUnaryExpr(const Unary& expr)
    {
        const TokenLiteral right = evaluate(expr.getRight());

        switch (expr.getOp().getTokenType())
        {
        case BANG:
            return !isTruthy(right);
        case MINUS:
            return -std::get<double>(right);
        default:
            break;
        }

        return {};
    }


    TokenLiteral Interpreter::evaluate(std::shared_ptr<Expr> expr)
    {
        return expr->accept(*this);
    }
}

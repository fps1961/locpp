#include "../include/Interpreter.h"
//
// Created by sheshan on 3/27/2025.
//
namespace lox {
    TokenLiteral Interpreter::visitGroupingExpr(const Grouping &expr) {
        return evaluate(expr.getExpression());
    }

    TokenLiteral Interpreter::visitLiteralExpr(const Literal &expr) {
        return expr.getValue();
    }

    TokenLiteral Interpreter::visitUnaryExpr(const Unary &expr) {
        const TokenLiteral right = evaluate(expr.getRight());

        switch (expr.getOp().getTokenType()) {
            case BANG:
                return !isTruthy(right);
            case MINUS:
                return -std::get<double>(right);
            default:
                break;
        }

        return {};
    }

    TokenLiteral Interpreter::visitBinaryExpr(const Binary &expr) {
        const TokenLiteral left = evaluate(expr.getLeft());
        const TokenLiteral right = evaluate(expr.getRight());

        switch (expr.getOp().getTokenType()) {
            case MINUS:
                return std::get<double>(left) - std::get<double>(right);
            case PLUS:
                if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
                    return std::get<double>(left) - std::get<double>(right);
                }

                if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)) {
                    return std::get<std::string>(left) + std::get<std::string>(right);
                }
                break;
            case SLASH:
                return std::get<double>(left) / std::get<double>(right);
            case STAR:
                return std::get<double>(left) * std::get<double>(right);
            default:
                break;
        }

        return nullptr;
    }


    bool Interpreter::isTruthy(TokenLiteral token_literal) const {
        return std::visit(TokenLiteralToBoolean{}, token_literal);
    }


    TokenLiteral Interpreter::evaluate(const std::shared_ptr<Expr> &expr) {
        return expr->accept(*this);
    }
}

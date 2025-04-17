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
            case BANG_EQUAL:
                return !isEqual(left, right);
            case EQUAL:
                return isEqual(left, right);
            case GREATER:
                return std::get<double>(left) > std::get<double>(right);
            case GREATER_EQUAL:
                return std::get<double>(left) >= std::get<double>(right);
            case LESS:
                return std::get<double>(left) < std::get<double>(right);
            case LESS_EQUAL:
                return std::get<double>(left) <= std::get<double>(right);
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

    bool Interpreter::isEqual(const TokenLiteral &a, const TokenLiteral &b) const {
        if (std::holds_alternative<std::monostate>(a) && std::holds_alternative<std::monostate>(b)) return true;

        if (std::holds_alternative<std::monostate>(a) || std::holds_alternative<std::monostate>(b)) return false;

        if (std::holds_alternative<std::string>(a) && std::holds_alternative<std::string>(b))
            return std::get<std::string>(a) == std::get<std::string>(b);

        if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b))
            return std::get<double>(a) == std::get<double>(b);

        if (std::holds_alternative<bool>(a) && std::holds_alternative<bool>(b))
            return std::get<bool>(a) == std::get<bool>(b);

        return false;
    }


    TokenLiteral Interpreter::evaluate(const std::shared_ptr<Expr> &expr) {
        return expr->accept(*this);
    }
}

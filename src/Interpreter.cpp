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

    bool Interpreter::isTruthy(TokenLiteral token_literal) const {
        return std::visit([]<typename T0>(const T0 &) -> bool {
            using T = std::decay_t<T0>;
            if constexpr (std::is_same_v<T, std::monostate> || std::is_same_v<T, bool>) {
                return false;
            } else {
                return true;
            }
        }, token_literal);
    }


    TokenLiteral Interpreter::evaluate(std::shared_ptr<Expr> expr) {
        return expr->accept(*this);
    }
}

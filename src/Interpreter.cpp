#include "../include/Interpreter.h"
#include <iostream>

#include "../include/Lox.h"
#include "../include/RuntimeError.h"
//
// Created by sheshan on 3/27/2025.
//
namespace lox {
    void Interpreter::interpret(const std::vector<std::shared_ptr<Stmt> > &statements) {
        try {
            for (auto statement: statements) {
                execute(statement);
            }
        } catch (RuntimeError &error) {
            Lox::runtimeError(error);
        }
    }

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
                checkNumberOperand(expr.getOp(), right);
                return -std::get<double>(right);
            default:
                break;
        }

        return {};
    }

    TokenLiteral Interpreter::visitVariableExpr(const Variable &expr) {
        return environment->get(expr.getName());
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
                checkNumberOperands(expr.getOp(), left, right);
                return std::get<double>(left) > std::get<double>(right);
            case GREATER_EQUAL:
                checkNumberOperands(expr.getOp(), left, right);
                return std::get<double>(left) >= std::get<double>(right);
            case LESS:
                checkNumberOperands(expr.getOp(), left, right);
                return std::get<double>(left) < std::get<double>(right);
            case LESS_EQUAL:
                checkNumberOperands(expr.getOp(), left, right);
                return std::get<double>(left) <= std::get<double>(right);
            case MINUS:
                checkNumberOperands(expr.getOp(), left, right);
                return std::get<double>(left) - std::get<double>(right);
            case PLUS:
                if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
                    return std::get<double>(left) + std::get<double>(right);
                }
                if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)) {
                    return std::get<std::string>(left) + std::get<std::string>(right);
                }
                throw RuntimeError{expr.getOp(), "Operands must be two numbers or two string."};
            case SLASH:
                checkNumberOperands(expr.getOp(), left, right);
                return std::get<double>(left) / std::get<double>(right);
            case STAR:
                checkNumberOperands(expr.getOp(), left, right);
                return std::get<double>(left) * std::get<double>(right);
            default:
                break;
        }

        return nullptr;
    }

    TokenLiteral Interpreter::visitPrintStmt(const Print &stmt) {
        TokenLiteral value = evaluate(stmt.getExpression());
        std::cout << stringify(value) << "\n";
        return {};
    }


    TokenLiteral Interpreter::visitVarStmt(const Var &stmt) {
        TokenLiteral value = nullptr;
        if (stmt.getInitializer() != nullptr) {
            value = evaluate(stmt.getInitializer());
        }
        environment->define(stmt.getName().getLexeme(), value);
        return {};
    }


    TokenLiteral Interpreter::visitExpressionStmt(const Expression &stmp) {
        evaluate(stmp.getExpression());
        return {};
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

    void Interpreter::checkNumberOperand(const Token &token, const TokenLiteral &operand) {
        if (std::holds_alternative<double>(operand)) return;
        throw RuntimeError{token, "Operand must be a number."};
    }

    void Interpreter::checkNumberOperands(const Token &token, const TokenLiteral &left, const TokenLiteral &right) {
        if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) return;
        throw RuntimeError{token, "Operands must be numbers."};
    }

    TokenLiteral Interpreter::evaluate(const std::shared_ptr<Expr> &expr) {
        return expr->accept(*this);
    }

    void Interpreter::execute(const std::shared_ptr<Stmt> &stmt) {
        stmt->accept(*this);
    }


    std::string Interpreter::stringify(TokenLiteral &object) {
        return std::visit(TokenLiteralToString{}, object);
    }
}

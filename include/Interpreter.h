//
// Created by sheshan on 3/27/2025.
//
#pragma once

#include <vector>

#include "Environment.h"
#include "Expr.h"
#include "Stmt.h"

namespace lox {
    class Interpreter : public ExprVisitor, public StmtVisitor {
    public:
        void interpret(const std::vector<std::shared_ptr<Stmt> > &statements);

        TokenLiteral visitGroupingExpr(const Grouping &expr) override;

        TokenLiteral visitLiteralExpr(const Literal &expr) override;

        TokenLiteral visitLogicalExpr(const Logical &expr) override;

        TokenLiteral visitUnaryExpr(const Unary &expr) override;

        TokenLiteral visitVariableExpr(const Variable &expr) override;

        TokenLiteral visitBinaryExpr(const Binary &expr) override;

        TokenLiteral visitCallExpr(const Call &expr) override;

        TokenLiteral visitExpressionStmt(const Expression &stmp) override;

        TokenLiteral visitIfStmt(const If &stmt) override;

        TokenLiteral visitPrintStmt(const Print &stmt) override;

        TokenLiteral visitVarStmt(const Var &stmt) override;

        TokenLiteral visitWhileStmt(const While &stmt) override;

        TokenLiteral visitAssignExpr(const Assign &expr) override;

        TokenLiteral visitBlockStmt(const Block &stmt) override;

    private:
        std::shared_ptr<Environment> environment{new Environment};

        [[nodiscard]] bool isTruthy(TokenLiteral token_literal) const;

        [[nodiscard]] bool isEqual(const TokenLiteral &a, const TokenLiteral &b) const;

        void checkNumberOperand(const Token &token, const TokenLiteral &operand);

        void checkNumberOperands(const Token &token, const TokenLiteral &left, const TokenLiteral &right);

        TokenLiteral evaluate(const std::shared_ptr<Expr> &expr);

        void execute(const std::shared_ptr<Stmt> &stmt);

        void executeBlock(const std::vector<std::shared_ptr<Stmt> > &statements,
                          const std::shared_ptr<Environment> &environment);

        std::string stringify(TokenLiteral &object);
    };
}

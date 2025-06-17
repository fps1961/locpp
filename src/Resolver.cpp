//
// Created by shesh on 6/11/2025.
//

#include "../include/Resolver.h"
#include <unordered_map>

#include "../include/Lox.h"

namespace lox {
    TokenLiteral Resolver::visitBlockStmt(const Block &stmt) {
        beginScope();
        resolve(stmt.getStatements());
        endScope();
        return {};
    }

    TokenLiteral Resolver::visitFunctionStmt(const Function &stmt) {
        declare(stmt.getName());
        define(stmt.getName());

        resolveFunction(stmt);
        return {};
    }

    TokenLiteral Resolver::visitVarStmt(const Var &stmt) {
        declare(stmt.getName());
        if (stmt.getInitializer() != nullptr) {
            resolve(stmt.getInitializer());
        }
        define(stmt.getName());
        return {};
    }

    TokenLiteral Resolver::visitVariableExpr(const Variable &expr) {
        if (!scopes.empty() && scopes.back().contains(expr.getName().getLexeme()) && scopes.back().at(
                expr.getName().getLexeme()) == false) {
            Lox::error(expr.getName(), "Can't read local variable in its own initialization.");
        }

        resolveLocal(std::make_shared<Variable>(expr), expr.name);
        return {};
    }

    TokenLiteral Resolver::visitAssignExpr(const Assign &expr) {
        resolve(expr.getValue());
        resolveLocal(std::make_shared<Assign>(expr), expr.getName());
        return {};
    }

    TokenLiteral Resolver::visitExpressionStmt(const Expression &stmt) {
        resolve(stmt.getExpression());
        return {};
    }

    TokenLiteral Resolver::visitIfStmt(const If &stmt) {
        resolve(stmt.getCondition());
        resolve(stmt.getThenBranch());
        if (stmt.getElseBranch() != nullptr) { resolve(stmt.getElseBranch()); }
        return {};
    }

    TokenLiteral Resolver::visitPrintStmt(const Print &stmt) {
        resolve(stmt.getExpression());
        return {};
    }

    TokenLiteral Resolver::visitReturnStmt(const Return &stmt) {
        if (stmt.getValue() != nullptr) {
            resolve(stmt.getValue());
        }
        return {};
    }

    TokenLiteral Resolver::visitWhileStmt(const While &stmt) {
        resolve(stmt.getCondition());
        resolve(stmt.getBody());
        return {};
    }

    TokenLiteral Resolver::visitBinaryExpr(const Binary &expr) {
        resolve(expr.getLeft());
        resolve(expr.getRight());
        return {};
    }

    TokenLiteral Resolver::visitCallExpr(const Call &expr) {
        resolve(expr.getCalle());

        for (const auto &argument: expr.getArguments()) {
            resolve(argument);
        }
        return {};
    }

    TokenLiteral Resolver::visitGroupingExpr(const Grouping &expr) {
        resolve(expr.getExpression());
        return {};
    }

    TokenLiteral Resolver::visitLiteralExpr(const Literal &expr) {
        return {};
    }

    TokenLiteral Resolver::visitLogicalExpr(const Logical &expr) {
        resolve(expr.getRight());
        resolve(expr.getLeft());
        return {};
    }


    TokenLiteral Resolver::visitUnaryExpr(const Unary &expr) {
        resolve(expr.getRight());
        return {};
    }


    void Resolver::resolve(const std::vector<std::shared_ptr<Stmt> > &stmts) {
        for (const auto &stmt: stmts) {
            resolve(stmt);
        }
    }

    void Resolver::resolve(const std::shared_ptr<Stmt> &stmt) {
        stmt->accept(*this);
    }

    void Resolver::resolve(const std::shared_ptr<Expr> &expr) {
        expr->accept(*this);
    }

    void Resolver::resolve(const Token &name) {
        if (scopes.empty()) { return; }
        auto &scope = scopes.back();
        scope[name.getLexeme()] = true;
    }

    void Resolver::resolveFunction(const Function &function) {
        beginScope();
        for (const auto &param: function.getParams()) {
            declare(param);
            define(param);
        }
        resolve(function.getBody());
        endScope();
    }


    void Resolver::resolveLocal(const std::shared_ptr<Expr> &expr, const Token &name) {
        for (int i = scopes.size() - 1; i >= 0; i--) {
            if (scopes[i].contains(name.getLexeme())) {
                interpreter.resolve(expr, scopes.size() - 1 - i);
            }
        }
    }

    void Resolver::declare(const Token &name) {
        if (scopes.empty()) {
            return;
        }
        auto &scope = scopes.back();
        scope[name.getLexeme()] = false;
    }

    void Resolver::define(const Token &name) {
        if (scopes.empty()) {
            return;
        }
        auto &scope = scopes.back();
        scope[name.getLexeme()] = true;
    }
};

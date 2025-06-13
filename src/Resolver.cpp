//
// Created by shesh on 6/11/2025.
//

#include "../include/Resolver.h"

#include <unordered_map>

namespace lox {
    TokenLiteral Resolver::visitBlockStmt(const Block &stmt) {
        beginScope();
        resolve(stmt.getStatements());
        endScope();
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

    void Resolver::declare(Token &name) {
        if (scopes.empty()) {
            return;
        }
        auto &scope = scopes.back();
        scope[name.getLexeme()] = false;
    }

    void Resolver::resolve(Token &name) {
        if (scopes.empty()) { return; }
        auto &scope = scopes.back();
        scope[name.getLexeme()] = true;
    }
};

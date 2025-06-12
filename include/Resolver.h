//
// Created by shesh on 6/10/2025.
//

#pragma once
#include <unordered_map>

#include "Expr.h"
#include "Stmt.h"

namespace lox {
    class Resolver : public StmtVisitor, ExprVisitor {
        Interpreter interpreter;
        std::vector<std::unordered_map<std::string, bool> > scopes;

    public:
        Resolver(Interpreter &interpreter) : interpreter(interpreter) {
        };

        TokenLiteral visitBlockStmt(const Block &stmt) override;

    private:
        void resolve(const std::vector<std::shared_ptr<Stmt> > &stmts);

        void resolve(const std::shared_ptr<Stmt> &stmt);

        void resolve(const std::shared_ptr<Expr> &expr);

        void beginScope() { scopes.push_back(std::unordered_map<std::string, bool>{}); };
        void endScope() { scopes.pop_back(); };
    };
}

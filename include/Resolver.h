//
// Created by shesh on 6/10/2025.
//

#pragma once
#include <unordered_map>

#include "Expr.h"
#include "Interpreter.h"
#include "Stmt.h"

namespace lox {
    class Resolver : public StmtVisitor, ExprVisitor {
        enum class FunctionType {
            NONE,
            FUNCTION,
        };

        Interpreter interpreter;
        std::vector<std::unordered_map<std::string, bool> > scopes;
        FunctionType currentFunction = FunctionType::NONE;

    public:
        explicit Resolver(const Interpreter &interpreter) : interpreter(interpreter) {
        };

        void resolve(const std::vector<std::shared_ptr<Stmt> > &stmts);


        TokenLiteral visitBlockStmt(const Block &stmt) override;

        TokenLiteral visitClassStmt(const Class &stmt) override;

        TokenLiteral visitFunctionStmt(const Function &stmt) override;

        TokenLiteral visitVarStmt(const Var &stmt) override;

        TokenLiteral visitVariableExpr(const Variable &expr) override;

        TokenLiteral visitAssignExpr(const Assign &expr) override;

        TokenLiteral visitExpressionStmt(const Expression &stmt) override;

        TokenLiteral visitIfStmt(const If &stmt) override;

        TokenLiteral visitPrintStmt(const Print &stmt) override;

        TokenLiteral visitReturnStmt(const Return &stmt) override;

        TokenLiteral visitWhileStmt(const While &stmt) override;

        TokenLiteral visitBinaryExpr(const Binary &expr) override;

        TokenLiteral visitCallExpr(const Call &expr) override;

        TokenLiteral visitGroupingExpr(const Grouping &expr) override;

        TokenLiteral visitLiteralExpr(const Literal &expr) override;

        TokenLiteral visitLogicalExpr(const Logical &expr) override;

        TokenLiteral visitUnaryExpr(const Unary &expr) override;

    private:
        void resolve(const std::shared_ptr<Stmt> &stmt);

        void resolve(const std::shared_ptr<Expr> &expr);

        void resolve(const Token &name);

        void resolveFunction(const Function &function, const FunctionType &type);

        void resolveLocal(const std::shared_ptr<Expr> &expr, const Token &name);

        void beginScope() { scopes.push_back(std::unordered_map<std::string, bool>{}); };
        void endScope() { scopes.pop_back(); };

        void declare(const Token &name);

        void define(const Token &name);
    };
}

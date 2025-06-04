#pragma once
#include <memory>
#include <variant>
#include "../include/Token.h"

namespace lox {
    class Block;
    class Expression;
    class Function;
    class If;
    class Print;
    class Var;
    class While;

    class StmtVisitor {
    public:
        virtual ~StmtVisitor() = default;

        virtual TokenLiteral visitBlockStmt(const Block &stmt) = 0;

        virtual TokenLiteral visitExpressionStmt(const Expression &stmt) = 0;

        virtual TokenLiteral visitFunctionStmt(const Function &stmt) = 0;

        virtual TokenLiteral visitIfStmt(const If &stmt) = 0;

        virtual TokenLiteral visitPrintStmt(const Print &stmt) = 0;

        virtual TokenLiteral visitVarStmt(const Var &stmt) = 0;

        virtual TokenLiteral visitWhileStmt(const While &stmt) = 0;
    };

    class Stmt {
    public:
        virtual ~Stmt() = default;

        virtual TokenLiteral accept(StmtVisitor &visitor) const = 0;
    };

    class Block final : public Stmt {
        std::vector<std::shared_ptr<Stmt> > statements;

    public:
        explicit Block(std::vector<std::shared_ptr<Stmt> > statements)
            : statements(std::move(statements)) {
        }

        TokenLiteral accept(StmtVisitor &visitor) const override {
            return visitor.visitBlockStmt(*this);
        }

        [[nodiscard]] const std::vector<std::shared_ptr<Stmt> > &getStatements() const { return statements; }
    };

    class Expression final : public Stmt {
        std::shared_ptr<Expr> expression;

    public:
        explicit Expression(std::shared_ptr<Expr> expression)
            : expression(std::move(expression)) {
        }

        TokenLiteral accept(StmtVisitor &visitor) const override {
            return visitor.visitExpressionStmt(*this);
        }

        [[nodiscard]] const std::shared_ptr<Expr> &getExpression() const { return expression; }
    };

    class Function final : public Stmt {
        Token name;
        std::vector<Token> params;
        std::vector<std::shared_ptr<Stmt> > body;

    public:
        Function(Token name, std::vector<Token> params, std::vector<std::shared_ptr<Stmt> > body)
            : name(std::move(name)), params(std::move(params)), body(std::move(body)) {
        }

        TokenLiteral accept(StmtVisitor &visitor) const override {
            return visitor.visitFunctionStmt(*this);
        }

        [[nodiscard]] const Token &getName() const { return name; }
        [[nodiscard]] const std::vector<Token> &getParams() const { return params; }
        [[nodiscard]] const std::vector<std::shared_ptr<Stmt> > &getBody() const { return body; }
    };

    class If final : public Stmt {
        std::shared_ptr<Expr> condition;
        std::shared_ptr<Stmt> thenBranch;
        std::shared_ptr<Stmt> elseBranch;

    public:
        If(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> thenBranch, std::shared_ptr<Stmt> elseBranch)
            : condition(std::move(condition)), thenBranch(std::move(thenBranch)), elseBranch(std::move(elseBranch)) {
        }

        TokenLiteral accept(StmtVisitor &visitor) const override {
            return visitor.visitIfStmt(*this);
        }

        [[nodiscard]] const std::shared_ptr<Expr> &getCondition() const { return condition; }
        [[nodiscard]] const std::shared_ptr<Stmt> &getThenBranch() const { return thenBranch; }
        [[nodiscard]] const std::shared_ptr<Stmt> &getElseBranch() const { return elseBranch; }
    };

    class Print final : public Stmt {
        std::shared_ptr<Expr> expression;

    public:
        explicit Print(std::shared_ptr<Expr> expression)
            : expression(std::move(expression)) {
        }

        TokenLiteral accept(StmtVisitor &visitor) const override {
            return visitor.visitPrintStmt(*this);
        }

        [[nodiscard]] const std::shared_ptr<Expr> &getExpression() const { return expression; }
    };

    class Var final : public Stmt {
        Token name;
        std::shared_ptr<Expr> initializer;

    public:
        Var(Token name, std::shared_ptr<Expr> initializer)
            : name(std::move(name)), initializer(std::move(initializer)) {
        }

        TokenLiteral accept(StmtVisitor &visitor) const override {
            return visitor.visitVarStmt(*this);
        }

        [[nodiscard]] const Token &getName() const { return name; }
        [[nodiscard]] const std::shared_ptr<Expr> &getInitializer() const { return initializer; }
    };

    class While final : public Stmt {
        std::shared_ptr<Expr> condition;
        std::shared_ptr<Stmt> body;

    public:
        While(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body)
            : condition(std::move(condition)), body(std::move(body)) {
        }

        TokenLiteral accept(StmtVisitor &visitor) const override {
            return visitor.visitWhileStmt(*this);
        }

        [[nodiscard]] const std::shared_ptr<Expr> &getCondition() const { return condition; }
        [[nodiscard]] const std::shared_ptr<Stmt> &getBody() const { return body; }
    };
}

#pragma once
#include <memory>
#include <variant>
#include "../include/Token.h"

namespace lox {
    class Expression;
    class Print;

    class StmtVisitor {
    public:
        virtual ~StmtVisitor() = default;

        virtual TokenLiteral visitExpressionStmt(const Expression &stmt) = 0;

        virtual TokenLiteral visitPrintStmt(const Print &stmt) = 0;
    };

    class Stmt {
    public:
        virtual ~Stmt() = default;

        virtual TokenLiteral accept(StmtVisitor &visitor) const = 0;
    };

    class Expression final : public Stmt {
        Expr expression;

    public:
        explicit Expression(Expr expression)
            : expression(std::move(expression)) {
        }

        TokenLiteral accept(StmtVisitor &visitor) const override {
            return visitor.visitExpressionStmt(*this);
        }

        [[nodiscard]] const Expr &getExpression() const { return expression; }
    };

    class Print final : public Stmt {
        Expr expression;

    public:
        explicit Print(Expr expression)
            : expression(std::move(expression)) {
        }

        TokenLiteral accept(StmtVisitor &visitor) const override {
            return visitor.visitPrintStmt(*this);
        }

        [[nodiscard]] const Expr &getExpression() const { return expression; }
    };
}

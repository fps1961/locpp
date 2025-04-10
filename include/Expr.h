#pragma once
#include <memory>
#include <variant>
#include "../include/Token.h"

namespace lox
{
    class Binary;
    class Grouping;
    class Literal;
    class Unary;

    class Visitor
    {
    public:
        virtual ~Visitor() = default;

        virtual TokenLiteral visitBinaryExpr(const Binary& expr) = 0;

        virtual TokenLiteral visitGroupingExpr(const Grouping& expr) = 0;

        virtual TokenLiteral visitLiteralExpr(const Literal& expr) = 0;

        virtual TokenLiteral visitUnaryExpr(const Unary& expr) = 0;
    };

    class Expr
    {
    public:
        virtual ~Expr() = default;

        virtual TokenLiteral accept(Visitor& visitor) const = 0;
    };

    class Binary final : public Expr
    {
        std::shared_ptr<Expr> left;
        Token op;
        std::shared_ptr<Expr> right;

    public:
        Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
            : left(std::move(left)), op(std::move(op)), right(std::move(right))
        {
        }

        TokenLiteral accept(Visitor& visitor) const override
        {
            return visitor.visitBinaryExpr(*this);
        }

        [[nodiscard]] const std::shared_ptr<Expr>& getLeft() const { return left; }
        [[nodiscard]] const Token& getOp() const { return op; }
        [[nodiscard]] const std::shared_ptr<Expr>& getRight() const { return right; }
    };

    class Grouping final : public Expr
    {
        std::shared_ptr<Expr> expression;

    public:
        explicit Grouping(std::shared_ptr<Expr> expression)
            : expression(std::move(expression))
        {
        }

        TokenLiteral accept(Visitor& visitor) const override
        {
            return visitor.visitGroupingExpr(*this);
        }

        [[nodiscard]] const std::shared_ptr<Expr>& getExpression() const { return expression; }
    };

    class Literal final : public Expr
    {
        TokenLiteral value;

    public:
        explicit Literal(TokenLiteral value)
            : value(std::move(value))
        {
        }

        TokenLiteral accept(Visitor& visitor) const override
        {
            return visitor.visitLiteralExpr(*this);
        }

        [[nodiscard]] const TokenLiteral& getValue() const { return value; }
    };

    class Unary final : public Expr
    {
        Token op;
        std::shared_ptr<Expr> right;

    public:
        Unary(Token op, std::shared_ptr<Expr> right)
            : op(std::move(op)), right(std::move(right))
        {
        }

        TokenLiteral accept(Visitor& visitor) const override
        {
            return visitor.visitUnaryExpr(*this);
        }

        [[nodiscard]] const Token& getOp() const { return op; }
        [[nodiscard]] const std::shared_ptr<Expr>& getRight() const { return right; }
    };
}

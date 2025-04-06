//
// Created by sheshan on 3/13/2025.
//

#include "AstPrinter.h"

#include "../../include/Expr.h"

namespace lox
{
    std::string AstPrinter::print(const std::shared_ptr<Expr>& expr)
    {
        return std::visit(TokenLiteralToString{}, expr->accept(*this));
    }

    TokenLiteral AstPrinter::visitBinaryExpr(const Binary& expr)
    {
        return parenthesize(expr.getOp().getLexeme(), expr.getLeft(), expr.getRight());
    }

    TokenLiteral AstPrinter::visitGroupingExpr(const Grouping& expr)
    {
        return parenthesize("group", expr.getExpression());
    }

    TokenLiteral AstPrinter::visitLiteralExpr(const Literal& expr)
    {
        return expr.getValue();
    }

    TokenLiteral AstPrinter::visitUnaryExpr(const Unary& expr)
    {
        return parenthesize(expr.getOp().getLexeme(), expr.getRight());
    }
}

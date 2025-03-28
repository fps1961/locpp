//
// Created by sheshan on 3/13/2025.
//

#include "AstPrinter.h"

#include "../../include/Expr.h"

namespace lox
{
    std::string AstPrinter::print(const std::shared_ptr<Expr>& expr)
    {
        expr->accept(*this);
        return result;
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
        //create a temp visitor struct so we can overload the () operator which is then called when we do std::visit
        return std::visit(TokenLiteralEvaluator{}, expr.getValue());
    }

    TokenLiteral AstPrinter::visitUnaryExpr(const Unary& expr)
    {
        return parenthesize(expr.getOp().getLexeme(), expr.getRight());
    }
}

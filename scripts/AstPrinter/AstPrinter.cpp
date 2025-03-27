//
// Created by shesh on 3/13/2025.
//

#include "AstPrinter.h"

#include <iostream>

#include "../../include/Expr.h"

namespace lox {
    std::string AstPrinter::print(const std::shared_ptr<Expr> &expr) {
        expr->accept(*this);
        return result;
    }

    void AstPrinter::visitBinaryExpr(const Binary &expr) {
        result = parenthesize(expr.getOp().getLexeme(), expr.getLeft(), expr.getRight());
    }

    void AstPrinter::visitGroupingExpr(const Grouping &expr) {
        result = parenthesize("group", expr.getExpression());
    }

    void AstPrinter::visitLiteralExpr(const Literal &expr) {
        //create a temp visitor struct so we can overload the () operator which is then called when we do std::visit
        struct TempLiteralVisitor {
            std::string operator()(std::monostate) const { return "nil"; }
            std::string operator()(const std::string &s) const { return s; }
            std::string operator()(const double d) const { return std::to_string(d); }
            std::string operator()(const bool b) const { return b ? "true" : "false"; }
        };
        result = std::visit(TempLiteralVisitor{}, expr.getValue());
    }

    void AstPrinter::visitUnaryExpr(const Unary &expr) {
        result = parenthesize(expr.getOp().getLexeme(), expr.getRight());
    }
}

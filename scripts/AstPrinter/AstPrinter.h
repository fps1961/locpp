//
// Created by sheshan on 3/19/2025.
//

#pragma once

#include <memory>
#include <sstream>
#include <string>
#include "../../include/Expr.h"

namespace lox {
    class AstPrinter final : public ExprVisitor {
    public:
        std::string print(const std::shared_ptr<Expr> &expr);

        TokenLiteral visitBinaryExpr(const Binary &expr) override;

        TokenLiteral visitGroupingExpr(const Grouping &expr) override;

        TokenLiteral visitLiteralExpr(const Literal &expr) override;

        TokenLiteral visitUnaryExpr(const Unary &expr) override;

    private:
        template<typename... E>
        std::string parenthesize(const std::string &name, E... expr) {
            std::ostringstream res{};
            res << "( " << name;
            ((res << " " << print(expr)), ...);
            res << ")";

            return res.str();
        }
    };
}

//
// Created by shesh on 3/19/2025.
//

#pragma once

#include <memory>
#include <sstream>
#include <string>
#include "../../include/Expr.h"

namespace lox {
    class AstPrinter final : public Visitor {
    public:
        std::string print(const std::shared_ptr<Expr> &expr);

        void visitBinaryExpr(const Binary &expr) override;

        void visitGroupingExpr(const Grouping &expr) override;

        void visitLiteralExpr(const Literal &expr) override;

        void visitUnaryExpr(const Unary &expr) override;

    private:
        std::string result{};

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

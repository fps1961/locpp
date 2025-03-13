//
// Created by shesh on 3/13/2025.
//

#include <iostream>
#include <sstream>

#include "../include/Expr.h"

class AstPrinter final : public lox::Visitor {
public:
    std::string print(const std::shared_ptr<lox::Expr> &expr) {
        expr->accept(*this);
        return result;
    }

    void visitBinaryExpr(const lox::Binary &expr) override {
        result = parenthesize(expr.getOp().getLexeme(), expr.getLeft(), expr.getRight());
    }

    void visitGroupingExpr(const lox::Grouping &expr) override {
        result = parenthesize("group", expr.getExpression());
    }

    void visitLiteralExpr(const lox::Literal &expr) override {
        //create a temp visitor struct so we can overload the () operator which is then called when we do std::visit
        struct TempLiteralVisitor {
            std::string operator()(std::nullptr_t) const { return "nil"; }
            std::string operator()(const bool value) const { return value ? "true" : "false"; }
            std::string operator()(const double value) const { return std::to_string(value); }
            std::string operator()(const std::string &value) const { return value; }
        };
        result = std::visit(TempLiteralVisitor{}, expr.getValue());
    }

    void visitUnaryExpr(const lox::Unary &expr) override {
        result = parenthesize(expr.getOp().getLexeme(), expr.getRight());
    }

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


int main() {
    const auto expression = std::make_shared<lox::Binary>(
        std::make_shared<lox::Unary>(lox::Token(lox::MINUS, "-", std::monostate{}, 1),
                                     std::make_shared<lox::Literal>(123.0)),
        lox::Token(lox::STAR, "*", std::monostate{}, 1),
        std::make_shared<lox::Grouping>(std::make_shared<lox::Literal>(45.67)));

    std::cout << AstPrinter().print(expression) << "\n";
}

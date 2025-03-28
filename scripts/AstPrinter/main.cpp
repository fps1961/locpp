//
// Created by sheshan on 3/26/2025.
//

#include <iostream>
#include <memory>
#include <variant>

#include "AstPrinter.h"

int main()
{
    const auto expression = std::make_shared<lox::Binary>(
        std::make_shared<lox::Unary>(lox::Token(lox::MINUS, "-", std::monostate{}, 1),
                                     std::make_shared<lox::Literal>(123.0)),
        lox::Token(lox::STAR, "*", std::monostate{}, 1),
        std::make_shared<lox::Grouping>(std::make_shared<lox::Literal>(45.67)));

    std::cout << lox::AstPrinter().print(expression) << "\n";
}

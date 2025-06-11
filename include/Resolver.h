//
// Created by shesh on 6/10/2025.
//

#pragma once
#include "Expr.h"
#include "Stmt.h"

namespace lox {
    class Resolver : public StmtVisitor, ExprVisitor {
        Interpreter interpreter;

    public:
        Resolver(Interpreter &interpreter) : interpreter(interpreter) {
        };
    };
}

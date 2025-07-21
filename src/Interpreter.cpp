#include "../include/Interpreter.h"

#include <chrono>
#include <iostream>
#include <variant>
#include <variant>

#include "../include/Lox.h"
#include "../include/LoxCallable.h"
#include "../include/LoxClass.h"
#include "../include/LoxFunction.h"
#include "../include/LoxReturn.h"
#include "../include/RuntimeError.h"
//
// Created by sheshan on 3/27/2025.
//
namespace lox {
    Interpreter::Interpreter() {
        globals->define("clock", std::shared_ptr<NativeClock>{});
    }

    void Interpreter::interpret(const std::vector<std::shared_ptr<Stmt> > &statements) {
        try {
            for (auto statement: statements) {
                execute(statement);
            }
        } catch (RuntimeError &error) {
            Lox::runtimeError(error);
        }
    }

    TokenLiteral Interpreter::visitGroupingExpr(const Grouping &expr) {
        return evaluate(expr.getExpression());
    }

    TokenLiteral Interpreter::visitLiteralExpr(const Literal &expr) {
        return expr.getValue();
    }

    TokenLiteral Interpreter::visitLogicalExpr(const Logical &expr) {
        TokenLiteral left = evaluate(expr.getLeft());
        if (expr.getOp().getTokenType() == OR) {
            if (isTruthy(left)) return left;
        } else {
            if (!isTruthy(left)) return left;
        }
        return evaluate(expr.getRight());
    }

    TokenLiteral Interpreter::visitSetExpr(const Set &expr) {
        auto object = evaluate(expr.getObject());

        if (const auto &loxInstance = std::get_if<std::shared_ptr<LoxInstance> >(&object)) {
            auto value = evaluate(expr.getValue());
            loxInstance->get()->set(expr.getName(), value);
            return value;
        }

        throw new RuntimeError(expr.getName(), "Only Instances have fields.");
    }


    TokenLiteral Interpreter::visitUnaryExpr(const Unary &expr) {
        const TokenLiteral right = evaluate(expr.getRight());

        switch (expr.getOp().getTokenType()) {
            case BANG:
                return !isTruthy(right);
            case MINUS:
                checkNumberOperand(expr.getOp(), right);
                return -std::get<double>(right);
            default:
                break;
        }

        return {};
    }

    TokenLiteral Interpreter::visitVariableExpr(const Variable &expr) {
        return lookUpVariable(expr.getName(), std::make_shared<Variable>(expr));
    }

    TokenLiteral Interpreter::visitBinaryExpr(const Binary &expr) {
        const TokenLiteral left = evaluate(expr.getLeft());
        const TokenLiteral right = evaluate(expr.getRight());

        switch (expr.getOp().getTokenType()) {
            case BANG_EQUAL:
                return !isEqual(left, right);
            case EQUAL:
                return isEqual(left, right);
            case GREATER:
                checkNumberOperands(expr.getOp(), left, right);
                return std::get<double>(left) > std::get<double>(right);
            case GREATER_EQUAL:
                checkNumberOperands(expr.getOp(), left, right);
                return std::get<double>(left) >= std::get<double>(right);
            case LESS:
                checkNumberOperands(expr.getOp(), left, right);
                return std::get<double>(left) < std::get<double>(right);
            case LESS_EQUAL:
                checkNumberOperands(expr.getOp(), left, right);
                return std::get<double>(left) <= std::get<double>(right);
            case MINUS:
                checkNumberOperands(expr.getOp(), left, right);
                return std::get<double>(left) - std::get<double>(right);
            case PLUS:
                if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
                    return std::get<double>(left) + std::get<double>(right);
                }
                if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)) {
                    return std::get<std::string>(left) + std::get<std::string>(right);
                }
                throw RuntimeError{expr.getOp(), "Operands must be two numbers or two string."};
            case SLASH:
                checkNumberOperands(expr.getOp(), left, right);
                return std::get<double>(left) / std::get<double>(right);
            case STAR:
                checkNumberOperands(expr.getOp(), left, right);
                return std::get<double>(left) * std::get<double>(right);
            default:
                break;
        }

        return {};
    }

    TokenLiteral Interpreter::visitCallExpr(const Call &expr) {
        TokenLiteral callee = evaluate(expr.getCalle());

        std::vector<TokenLiteral> arguments{};

        for (const auto &argument: expr.getArguments()) {
            arguments.push_back(evaluate(argument));
        }

        if (const auto callable = std::get_if<std::shared_ptr<LoxCallable> >(&callee)) {
            if (static_cast<int>(arguments.size()) != callable->get()->arity()) {
                throw new RuntimeError(expr.getParen(),
                                       "Expected " + std::to_string(callable->get()->arity()) + " arguments but got " +
                                       std::to_string(arguments.size())
                                       + ".");
            }
            return callable->get()->call(*this, arguments);
        }
        throw new RuntimeError(expr.getParen(), "Can only call functions and classes.");
    }

    TokenLiteral Interpreter::visitGetExpr(const Get &expr) {
        TokenLiteral object = evaluate(expr.getObject());
        if (const auto &loxInstance = std::get_if<std::shared_ptr<LoxInstance> >(&object)) {
            return loxInstance->get()->get(expr.getName());
        }

        throw new RuntimeError(expr.getName(), "Only instances have properties");
    }


    TokenLiteral Interpreter::visitPrintStmt(const Print &stmt) {
        TokenLiteral value = evaluate(stmt.getExpression());
        std::cout << stringify(value) << "\n";
        return {};
    }

    TokenLiteral Interpreter::visitReturnStmt(const Return &stmt) {
        TokenLiteral value{};
        if (stmt.getValue() != nullptr) value = evaluate(stmt.getValue());

        throw LoxReturn{value};
    }


    TokenLiteral Interpreter::visitVarStmt(const Var &stmt) {
        TokenLiteral value = std::monostate{};
        if (stmt.getInitializer() != nullptr) {
            value = evaluate(stmt.getInitializer());
        }
        environment->define(stmt.getName().getLexeme(), value);
        return {};
    }

    TokenLiteral Interpreter::visitWhileStmt(const While &stmt) {
        while (isTruthy(evaluate(stmt.getCondition()))) {
            execute(stmt.getBody());
        }
        return {};
    }


    TokenLiteral Interpreter::visitAssignExpr(const Assign &expr) {
        TokenLiteral value = evaluate(expr.getValue());
        environment->assign(expr.getName(), value);
        return value;
    }


    TokenLiteral Interpreter::visitExpressionStmt(const Expression &stmp) {
        evaluate(stmp.getExpression());
        return {};
    }

    TokenLiteral Interpreter::visitFunctionStmt(const Function &stmt) {
        auto function = std::make_shared<LoxFunction>(std::make_shared<Function>(stmt),
                                                      std::make_shared<Environment>(environment));
        environment->define(stmt.getName().getLexeme(), function);
        return {};
    }


    TokenLiteral Interpreter::visitIfStmt(const If &stmt) {
        if (isTruthy(evaluate(stmt.getCondition()))) {
            execute(stmt.getThenBranch());
        } else if (stmt.getElseBranch() != nullptr) {
            execute(stmt.getElseBranch());
        }
        return {};
    }


    TokenLiteral Interpreter::visitBlockStmt(const Block &stmt) {
        executeBlock(stmt.getStatements(), std::make_shared<Environment>(environment));
        return {};
    }

    TokenLiteral Interpreter::visitClassStmt(const Class &stmt) {
        environment->define(stmt.getName().getLexeme(), {});
        TokenLiteral klass = std::make_shared<LoxClass>(stmt.getName().getLexeme());
        environment->assign(stmt.getName(), klass);
        return {};
    }


    bool Interpreter::isTruthy(TokenLiteral token_literal) const {
        return std::visit(TokenLiteralToBoolean{}, token_literal);
    }

    bool Interpreter::isEqual(const TokenLiteral &a, const TokenLiteral &b) const {
        if (std::holds_alternative<std::monostate>(a) && std::holds_alternative<std::monostate>(b)) return true;

        if (std::holds_alternative<std::monostate>(a) || std::holds_alternative<std::monostate>(b)) return false;

        if (std::holds_alternative<std::string>(a) && std::holds_alternative<std::string>(b))
            return std::get<std::string>(a) == std::get<std::string>(b);

        if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b))
            return std::get<double>(a) == std::get<double>(b);

        if (std::holds_alternative<bool>(a) && std::holds_alternative<bool>(b))
            return std::get<bool>(a) == std::get<bool>(b);

        return false;
    }

    void Interpreter::checkNumberOperand(const Token &token, const TokenLiteral &operand) {
        if (std::holds_alternative<double>(operand)) return;
        throw RuntimeError{token, "Operand must be a number."};
    }

    void Interpreter::checkNumberOperands(const Token &token, const TokenLiteral &left, const TokenLiteral &right) {
        if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) return;
        throw RuntimeError{token, "Operands must be numbers."};
    }

    TokenLiteral Interpreter::evaluate(const std::shared_ptr<Expr> &expr) {
        return expr->accept(*this);
    }

    void Interpreter::execute(const std::shared_ptr<Stmt> &stmt) {
        stmt->accept(*this);
    }

    void Interpreter::resolve(const std::shared_ptr<Expr> &expr, int depth) {
        locals.insert({expr, depth});
    }


    void Interpreter::executeBlock(const std::vector<std::shared_ptr<Stmt> > &statements,
                                   const std::shared_ptr<Environment> &environment) {
        auto previous = this->environment;
        try {
            this->environment = environment;
            for (const auto &statement: statements) {
                execute(statement);
            }
        } catch (...) {
            this->environment = previous;
            throw;
        }

        this->environment = previous;
    }


    std::string Interpreter::stringify(TokenLiteral &object) {
        return std::visit(TokenLiteralToString{}, object);
    }

    TokenLiteral Interpreter::lookUpVariable(const Token &name, const std::shared_ptr<Variable> &token) {
        if (const auto distance = locals.find(token); distance != locals.end()) {
            return environment->getAt(distance->second, name.getLexeme());
        } else {
            return globals->get(name);
        }
    }


    int NativeClock::arity() {
        return 0;
    }

    TokenLiteral NativeClock::call() {
        const auto ticks = std::chrono::system_clock::now().time_since_epoch();
        return std::chrono::duration<double>(ticks).count() / 1000.0;
    }

    TokenLiteral NativeClock::call(Interpreter &/*interpreter*/, std::vector<TokenLiteral> &/*arguments*/) {
        return call();
    }


    std::string NativeClock::toString() {
        return "<native fn>";
    }
}

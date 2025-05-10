//
// Created by sheshan on 3/6/2025.
//

#pragma once
#include <string>

#include "Interpreter.h"
#include "RuntimeError.h"
#include "Token.h"

namespace lox {
    class Lox {
    public:
        static void main(int argc, char *argv[]);

        static void error(int line, const std::string &message);

        static void error(const Token &token, const std::string &message);

        static void runtimeError(RuntimeError error);

    private:
        static bool hadError;
        static bool hadRuntimeError;
        static Interpreter interpreter;

        static void readFile(const std::string &filename);

        static void runPrompt();

        static void run(const std::string &input);

        static void report(int line, const std::string &where, const std::string &message);
    };
}

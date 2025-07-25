//
// Created by sheshan on 3/6/2025.
//


#include "../include/Lox.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "../include/Parser.h"
#include "../include/Resolver.h"
#include "../include/Scanner.h"
#include "../scripts/AstPrinter/AstPrinter.h"


namespace lox {
    void Lox::main(const int argc, char *argv[]) {
        if (argc > 2) {
            std::cout << "Usage: locpp [script]";
            exit(64);
        }
        if (argc == 2) {
            readFile(argv[1]);
        } else {
            runPrompt();
        }
    }

    void Lox::readFile(const std::string &filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file) {
            throw std::runtime_error("Error reading file: " + filename);
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        run(buffer.str());

        if (hadError) exit(65);
        if (hadRuntimeError) exit(70);
    }

    void Lox::runPrompt() {
        std::string line;

        while (true) {
            std::cout << "> ";
            if (!std::getline(std::cin, line)) break;
            run(line);
            hadError = false;
        }
    }

    void Lox::run(const std::string &input) {
        Scanner scanner{input};
        const std::vector<Token> tokens = scanner.scanTokens();
        Parser parser{tokens};
        const std::vector<std::shared_ptr<Stmt> > statements = parser.parse();

        if (hadError) return;

        Resolver resolver{interpreter};
        resolver.resolve(statements);
        if (hadError) return;

        interpreter.interpret(statements);
    }

    void Lox::error(const int line, const std::string &message) {
        report(line, "", message);
    }

    void Lox::runtimeError(RuntimeError error) {
        std::cout << error.what() << "\n[line " << error.token.getLine() << "]\n";
        hadRuntimeError = true;
    }


    void Lox::report(const int line, const std::string &where, const std::string &message) {
        std::cerr << "[line " << line << "] Error" << where << ": " << message << std::endl;
        hadError = true;
    }

    void Lox::error(const Token &token, const std::string &message) {
        if (token.getTokenType() == EOF) {
            report(token.getLine(), " at end", message);
        } else {
            report(token.getLine(), " at '" + token.getLexeme() + "'", message);
        }
    }


    bool Lox::hadError = false;
    bool Lox::hadRuntimeError = false;
    Interpreter Lox::interpreter{};
}

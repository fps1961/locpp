//
// Created by shesh on 3/6/2025.
//


#include "../include/lox.h"

#include <fstream>
#include <iostream>
#include <sstream>

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
        auto scanner = Scanner(input);

        for (auto tokens = scanner.scanTokens(); const auto &token: tokens) {
            std::cout << token;
        }
    }

    void Lox::error(const int line, const std::string &message) {
        report(line, "", message);
    }


    void Lox::report(const int line, const std::string &where, const std::string &message) {
        std::cerr << "[line " << line << "] Error" << where << ": " << message << std::endl;
        hadError = true;
    }

    bool Lox::hadError = false;
}

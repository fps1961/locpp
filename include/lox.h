//
// Created by shesh on 3/6/2025.
//

#pragma once
#include <string>

namespace lox {
    class Lox {
    public:
        static void main(int argc, char *argv[]);

        static void error(int line, const std::string &message);

    private:
        static bool hadError;

        static void readFile(const std::string &filename);

        static void runPrompt();

        static void run(const std::string &input);


        static void report(int line, const std::string &where, const std::string &message);
    };
}

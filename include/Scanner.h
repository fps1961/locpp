//
// Created by shesh on 3/8/2025.
//

#pragma once
#include <string>
#include <vector>

#include "Token.h"
#include "TokenType.h"

namespace lox {
    class Scanner {
    public:
        explicit Scanner(std::string source);

        std::vector<Token> scanTokens();

    private:
        std::string source;
        std::vector<Token> tokens{};
        int start{0};
        int current{0};
        int line{1};

        [[nodiscard]] bool isAtEnd() const;

        void scanToken();

        bool match(char expectedChar);

        char advance();

        void addToken(TokenType tokenType);

        void addToken(TokenType token, const Literal &literal);
    };
}

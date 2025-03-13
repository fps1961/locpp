//
// Created by shesh on 3/8/2025.
//

#pragma once
#include <string>
#include <unordered_map>
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
        static const std::unordered_map<std::string, TokenType> keywords;


        [[nodiscard]] bool isAtEnd() const;

        void scanToken();

        void identifier();

        void number();

        void string();

        bool match(char expectedChar);

        [[nodiscard]] char peek() const;

        [[nodiscard]] char peekNext() const;

        [[nodiscard]] static bool isAlpha(char c);

        [[nodiscard]] static bool isAlphanumeric(char c);

        [[nodiscard]] static bool isDigit(char c);


        char advance();

        void addToken(TokenType tokenType);

        void addToken(TokenType token, const TokenLiteral &literal);
    };
}

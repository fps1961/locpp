//
// Created by shesh on 3/8/2025.
//

#include "../include/Scanner.h"

#include <iostream>

#include "../include/lox.h"

namespace lox {
    Scanner::Scanner(std::string source) : source(std::move(source)) {
    }

    std::vector<Token> Scanner::scanTokens() {
        while (!isAtEnd()) {
            start = current;
            scanToken();
        }

        tokens.emplace_back(END_OF_FILE, "", nullptr, line);
        return tokens;
    }

    void Scanner::scanToken() {
        switch (advance()) {
            case '(':
                addToken(LEFT_PAREN);
                break;
            case ')':
                addToken(RIGHT_PAREN);
                break;
            case '{':
                addToken(LEFT_BRACE);
                break;
            case '}':
                addToken(RIGHT_BRACE);
                break;
            case ',':
                addToken(COMMA);
                break;
            case '.':
                addToken(DOT);
                break;
            case '-':
                addToken(MINUS);
                break;
            case '+':
                addToken(PLUS);
                break;
            case ';':
                addToken(SEMICOLON);
                break;
            case '*':
                addToken(STAR);
                break;
            case '!':
                addToken(match('=') ? BANG_EQUAL : BANG);
                break;
            case '=':
                addToken(match('=') ? EQUAL_EQUAL : EQUAL);
                break;
            case '<':
                addToken(match('<') ? LESS_EQUAL : LESS);
                break;
            case '>':
                addToken(match('>') ? GREATER_EQUAL : GREATER);
                break;
            default:
                Lox::error(line, "unexpected character.");
                break;
        }
    }

    bool Scanner::match(const char expectedChar) {
        if (isAtEnd()) return false;
        if (source.at(current) != expectedChar) return false;

        ++current;
        return true;
    }


    bool Scanner::isAtEnd() const {
        return current >= source.size();
    }

    char Scanner::advance() {
        return source.at(current++);
    }

    void Scanner::addToken(const TokenType tokenType) {
        addToken(tokenType, nullptr);
    }

    void Scanner::addToken(TokenType token, const Literal &literal) {
        std::string text = source.substr(start, current - start);
        tokens.emplace_back(token, text, literal, line);
    }
}

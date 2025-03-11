//
// Created by shesh on 3/8/2025.
//

#include "../include/Scanner.h"

#include <iostream>
#include <unordered_map>

#include "../include/lox.h"

namespace lox {
    Scanner::Scanner(std::string source) : source(std::move(source)) {
    }

    std::vector<Token> Scanner::scanTokens() {
        while (!isAtEnd()) {
            start = current;
            scanToken();
        }

        tokens.emplace_back(END_OF_FILE, "", std::monostate{}, line);
        return tokens;
    }

    void Scanner::scanToken() {
        switch (const char c = advance()) {
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
                addToken(match('=') ? LESS_EQUAL : LESS);
                break;
            case '>':
                addToken(match('=') ? GREATER_EQUAL : GREATER);
                break;
            case '/':
                if (match('/')) {
                    while (peek() != '\n' && !isAtEnd()) advance();
                } else {
                    addToken(SLASH);
                }
                break;
            case ' ':
            case '\r':
            case '\t':
                break;
            case '\n':
                ++line;
                break;
            case '"':
                string();
                break;
            default:
                if (isDigit(c)) {
                    number();
                } else if (isAlpha(c)) {
                    identifier();
                } else {
                    Lox::error(line, "unexpected character.");
                }
                break;
        }
    }

    void Scanner::identifier() {
        while (isAlphanumeric(peek())) { advance(); }

        const std::string value = source.substr(start, current - start);
        TokenType type = IDENTIFIER;

        if (const auto tokenType = keywords.find(value); tokenType != keywords.end()) {
            type = tokenType->second;
        }
        addToken(type);
    }


    void Scanner::number() {
        while (isDigit(peek())) { advance(); }

        if (peek() == '.' && isDigit(peekNext())) {
            advance();
            while (isDigit(peek())) { advance(); }
        }

        addToken(NUMBER, std::stod(source.substr(start, current - start)));
    }

    void Scanner::string() {
        while (peek() != '"' && !isAtEnd()) {
            if (peek() == '\n') ++line;
            advance();
        }

        if (isAtEnd()) {
            Lox::error(line, "Unterminated string.");
            return;
        }

        advance();
        std::string value = source.substr(start + 1, current - start - 2);
        addToken(STRING, value);
    }


    bool Scanner::match(const char expectedChar) {
        if (isAtEnd()) return false;
        if (source[current] != expectedChar) return false;

        ++current;
        return true;
    }


    bool Scanner::isAtEnd() const {
        return current >= static_cast<int>(source.size());
    }

    char Scanner::advance() {
        return source[current++];
    }

    void Scanner::addToken(const TokenType tokenType) {
        addToken(tokenType, std::monostate{});
    }

    void Scanner::addToken(TokenType token, const Literal &literal) {
        std::string text = source.substr(start, current - start);
        tokens.emplace_back(token, text, literal, line);
    }

    char Scanner::peek() const {
        if (isAtEnd()) return '\0';
        return source[current];
    }

    char Scanner::peekNext() const {
        if (current + 1 >= static_cast<int>(source.size())) return '\0';
        return source[current + 1];
    }

    bool Scanner::isAlpha(const char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
    }

    bool Scanner::isAlphanumeric(const char c) {
        return isDigit(c) || isAlpha(c);
    }


    bool Scanner::isDigit(const char c) {
        return c >= '0' && c <= '9';
    }


    const std::unordered_map<std::string, TokenType> Scanner::keywords =
    {
        {"and", AND},
        {"class", CLASS},
        {"else", ELSE},
        {"false", FALSE},
        {"for", FOR},
        {"fun", FUN},
        {"if", IF},
        {"nil", NIL},
        {"or", OR},
        {"print", PRINT},
        {"return", RETURN},
        {"super", SUPER},
        {"this", THIS},
        {"true", TRUE},
        {"var", VAR},
        {"while", WHILE},
    };
}

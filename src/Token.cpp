//
// Created by sheshan on 3/7/2025.
//


#include <utility>

#include "../include/Token.h"

#include <sstream>

namespace lox
{
    Token::Token(const TokenType type, std::string lexeme, TokenLiteral literal, const int line) : type(type),
        lexeme(std::move(lexeme)),
        literal(std::move(literal)), line(line)
    {
    }

    const std::string& Token::getLexeme() const
    {
        return lexeme;
    }

    const TokenType& Token::getTokenType() const
    {
        return type;
    }

    const TokenLiteral& Token::getLiteral() const
    {
        return literal;
    }

    int Token::getLine() const
    {
        return line;
    }


    std::string Token::toString() const
    {
        std::ostringstream oss;
        oss << tokenTypeToString(type) << " " << lexeme << " ";

        switch (type)
        {
        case IDENTIFIER:
            oss << lexeme;
            break;
        case STRING:
            oss << std::get<std::string>(literal);
            break;
        case NUMBER:
            oss << std::get<double>(literal);
            break;
        case TRUE:
            oss << "true";
            break;
        case FALSE:
            oss << "false";
            break;
        default:
            oss << "nil";
            break;
        }


        return oss.str();
    }

    std::ostream& operator<<(std::ostream& os, const Token& token)
    {
        return os << token.toString();
    }
}

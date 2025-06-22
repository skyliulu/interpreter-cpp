#include "token.h"
#include <iostream>

Token::Token(TokenType t, const std::string &lexeme, const std::optional<Literal> literal, int line)
    : type(t), lexeme(lexeme), literal(literal), line(line) {}

Token::~Token() {}

TokenType Token::get_type() const
{
    return type;
}
std::string Token::get_lexeme() const
{
    return lexeme;
}
const std::optional<Literal> &Token::get_literal() const
{
    return literal;
}
int Token::get_line() const
{
    return line;
}

std::ostream &operator<<(std::ostream &os, const Token &token)
{
    // Output the token type, lexeme, and literal value
    os << TokentypeToString(token.type) << " " << token.get_lexeme() << " ";
    if (token.get_literal().has_value())
    {
        // Use std::visit to handle the different types in the variant
        std::visit([&os](const auto &value)
                   {
            using T = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<T, std::nullptr_t>) {
                os << "nil"; // Output "nil" for nullptr_t
            } else {
                os << value; // Output other types directly (string, double, bool)
            } }, token.get_literal().value());
    }
    else
    {
        os << "null";
    }
    return os;
}

std::string TokentypeToString(TokenType type)
{
    switch (type)
    {
    case LEFT_PAREN:
        return "LEFT_PAREN";
    case RIGHT_PAREN:
        return "RIGHT_PAREN";
    case LEFT_BRACE:
        return "LEFT_BRACE";
    case RIGHT_BRACE:
        return "RIGHT_BRACE";
    case COMMA:
        return "COMMA";
    case DOT:
        return "DOT";
    case MINUS:
        return "MINUS";
    case PLUS:
        return "PLUS";
    case SEMICOLON:
        return "SEMICOLON";
    case SLASH:
        return "SLASH";
    case STAR:
        return "STAR";
    case BANG:
        return "BANG";
    case BANG_EQUAL:
        return "BANG_EQUAL";
    case EQUAL:
        return "EQUAL";
    case EQUAL_EQUAL:
        return "EQUAL_EQUAL";
    case GREATER:
        return "GREATER";
    case GREATER_EQUAL:
        return "GREATER_EQUAL";
    case LESS:
        return "LESS";
    case LESS_EQUAL:
        return "LESS_EQUAL";
    case IDENTIFIER:
        return "IDENTIFIER";
    case STRING:
        return "STRING";
    case NUMBER:
        return "NUMBER";
    case AND:
        return "AND";
    case CLASS:
        return "CLASS";
    case ELSE:
        return "ELSE";
    case FALSE:
        return "FALSE";
    case FUN:
        return "FUN";
    case FOR:
        return "FOR";
    case IF:
        return "IF";
    case NIL:
        return "NIL";
    case OR:
        return "OR";
    case PRINT:
        return "PRINT";
    case RETURN:
        return "RETURN";
    case SUPER:
        return "SUPER";
    case THIS:
        return "THIS";
    case TRUE:
        return "TRUE";
    case VAR:
        return "VAR";
    case WHILE:
        return "WHILE";
    case EOF_TOKEN:
        return "EOF";
    default:
        return "UNKNOWN_TOKEN";
    }
}
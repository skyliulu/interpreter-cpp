#include <string>
#include "token.h"
#include <iostream>
#include <optional>

Token::Token(Tokentype type, const std::string &lexeme, const std::optional<std::string> literal, int line)
    : type(type), lexeme(lexeme), literal(literal), line(line) {}

Token::~Token() {}

Tokentype Token::get_type() const
{
    return type;
}
std::string Token::get_lexeme() const
{
    return lexeme;
}
std::string Token::get_literal() const
{
    return literal.has_value() ? literal.value() : "null";
}
int Token::get_line() const
{
    return line;
}

std::ostream &operator<<(std::ostream &os, const Token &token)
{
    // Output the token type, lexeme, and literal value
    os << TokentypeToString(token.type)
       << " " << token.get_lexeme()
       << " " << token.get_literal();
    return os;
}

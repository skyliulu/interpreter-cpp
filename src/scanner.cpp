#include <string>
#include "scanner.h"

Scanner::Scanner(const std::string &source) : source(source) {}

Scanner::~Scanner() {}

std::vector<Token> Scanner::scan_tokens()
{
    while (!is_at_end())
    {
        start = current;
        scan_token();
    }
    tokens.push_back(Token(TokenType::EOF_TOKEN, "", std::nullopt, line));
    return tokens;
}

void Scanner::scan_token()
{
    char ch = advance();
    switch (ch)
    {
    case '(':
        /* code */
        add_token(TokenType::LEFT_PAREN);
        break;
    case ')':
        add_token(TokenType::RIGHT_PAREN);
        break;
    case '{':
        add_token(TokenType::LEFT_BRACE);
        break;
    case '}':
        add_token(TokenType::RIGHT_BRACE);
        break;
    default:
        break;
    }
}

bool Scanner::is_at_end() const
{
    return current >= source.length();
}

char Scanner::advance()
{
    return source[current++];
}

char Scanner::peek() const
{
    if (is_at_end())
    {
        return '\0';
    }
    return source[current];
}

char Scanner::peek_next() const
{
    if (current + 1 >= source.length())
    {
        return '\0';
    }
    return source[current + 1];
}

bool Scanner::match(char expect)
{
    if (is_at_end())
    {
        return false;
    }
    if (source[current] != expect)
    {
        return false;
    }
    current++;
    return true;
}

void Scanner::add_token(TokenType type, const std::optional<std::string> literal)
{
    Token token = Token(type, source.substr(start, current - start), literal, line);
    tokens.push_back(token);
}
#include "parser.h"
#include "lox.h"
#include "expr.h"

Parser::Parser(const std::vector<Token> &token) : tokens(token), current(0) {}

Parser::~Parser() {}

std::vector<std::unique_ptr<Expr>> Parser::parse()
{
    try
    {
        std::vector<std::unique_ptr<Expr>> expressions;
        while (!is_at_end())
        {
            expressions.push_back(expresstion());
        }
        return expressions;
    }
    catch (const ParserError &e)
    {
        synchronize(); // Attempt to recover from the error
        return {}; // Return an empty vector if parsing fails
    }
}

std::unique_ptr<Expr> Parser::expresstion()
{
    return equality();
}

std::unique_ptr<Expr> Parser::equality()
{
    std::unique_ptr<Expr> left = comparison();

    while (match({TokenType::EQUAL_EQUAL, TokenType::BANG_EQUAL}))
    {
        Token operator_ = previous();
        std::unique_ptr<Expr> right = comparison();
        left = std::make_unique<Expr::Binary>(std::move(left), operator_, std::move(right));
    }

    return left;
}

std::unique_ptr<Expr> Parser::comparison()
{
    std::unique_ptr<Expr> left = term();

    while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL}))
    {
        Token operator_ = previous();
        std::unique_ptr<Expr> right = term();
        left = std::make_unique<Expr::Binary>(std::move(left), operator_, std::move(right));
    }

    return left;
}

std::unique_ptr<Expr> Parser::term()
{
    std::unique_ptr<Expr> left = factor();

    while (match({TokenType::PLUS, TokenType::MINUS}))
    {
        Token operator_ = previous();
        std::unique_ptr<Expr> right = factor();
        left = std::make_unique<Expr::Binary>(std::move(left), operator_, std::move(right));
    }

    return left;
}

std::unique_ptr<Expr> Parser::factor()
{
    std::unique_ptr<Expr> left = unary();

    while (match({TokenType::STAR, TokenType::SLASH}))
    {
        Token operator_ = previous();
        std::unique_ptr<Expr> right = unary();
        left = std::make_unique<Expr::Binary>(std::move(left), operator_, std::move(right));
    }

    return left;
}

std::unique_ptr<Expr> Parser::unary()
{
    if (match({TokenType::MINUS, TokenType::BANG}))
    {
        Token operator_ = previous();
        std::unique_ptr<Expr> right = unary();
        return std::make_unique<Expr::Unary>(operator_, std::move(right));
    }
    return primary();
}

std::unique_ptr<Expr> Parser::primary()
{
    if (match({TokenType::FALSE}))
    {
        return std::make_unique<Expr::Literal>(previous(), false);
    }
    if (match({TokenType::TRUE}))
    {
        return std::make_unique<Expr::Literal>(previous(), true);
    }
    if (match({TokenType::NIL}))
    {
        return std::make_unique<Expr::Literal>(previous(), nullptr);
    }
    if (match({TokenType::NUMBER}))
    {
        Token token = previous();
        return std::make_unique<Expr::Literal>(token, token.get_literal().value_or(0.0));
    }
    if (match({TokenType::STRING}))
    {
        Token token = previous();
        return std::make_unique<Expr::Literal>(token, token.get_literal().value_or(""));
    }
    if (match({TokenType::LEFT_PAREN}))
    {
        std::unique_ptr<Expr> expr = expresstion();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_unique<Expr::Grouping>(std::move(expr));
    }

    throw error(peek(), "Expect expression.");
}

bool Parser::is_at_end()
{
    return peek().get_type() == TokenType::EOF_TOKEN;
}

bool Parser::check(TokenType type)
{
    if (is_at_end())
        return false;
    return tokens[current].get_type() == type;
}

bool Parser::check_next(TokenType type)
{
    if (current + 1 >= tokens.size())
        return false;
    return tokens[current + 1].get_type() == type;
}

Token Parser::peek()
{
    return tokens[current];
}

Token Parser::previous()
{
    return tokens[current - 1];
}

Token Parser::advance()
{
    if (!is_at_end())
        current++;
    return previous();
}

bool Parser::match(const std::vector<TokenType> &types)
{
    for (const auto &type : types)
    {
        if (check(type))
        {
            advance();
            return true;
        }
    }
    return false;
}

ParserError Parser::error(Token token, const std::string &message)
{
    Lox::error(token, message);
    // Create a ParserError with the message and token information
    return ParserError("Error at " + token.get_lexeme() + ": " + message);
}

Token Parser::consume(TokenType type, const std::string &message)
{
    if (check(type))
    {
        return advance();
    }
    throw error(peek(), message);
}

void Parser::synchronize()
{
    advance();
    while (!is_at_end())
    {
        if (previous().get_type() == TokenType::SEMICOLON)
            return;

        switch (peek().get_type())
        {
        case TokenType::CLASS:
        case TokenType::FUN:
        case TokenType::VAR:
        case TokenType::FOR:
        case TokenType::IF:
        case TokenType::WHILE:
        case TokenType::PRINT:
        case TokenType::RETURN:
            return;
        default:
            break;
        }
        advance();
    }
}
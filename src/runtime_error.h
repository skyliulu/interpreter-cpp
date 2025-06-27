#pragma once
#include <stdexcept>
#include <string>
#include <any>
#include "token.h"

class RuntimeError : public std::runtime_error
{
public:
    RuntimeError(const Token &token, const std::string &message)
        : std::runtime_error(message), token(token) {}

    Token get_token() const
    {
        return token;
    }

private:
    Token token; // Line number where the error occurred
};

class Return : public std::runtime_error
{
public:
    Return(const std::any &value)
        : std::runtime_error(""), value(value) {}

    std::any get_value() const
    {
        return value;
    }

private:
    const std::any value;
};
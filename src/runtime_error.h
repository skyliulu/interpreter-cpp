#pragma once
#include <stdexcept>
#include <string>
#include "token.h"

class RuntimeError : public std::runtime_error
{
public:
    RuntimeError(const Token &token, const std::string &message)
        : std::runtime_error("Runtime Error at line " + std::to_string(token.get_line()) + ": " + message), token(token) {}

    Token get_token() const
    {
        return token;
    }

private:
    Token token; // Line number where the error occurred
};
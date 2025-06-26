#pragma once
#include <unordered_map>
#include <any>
#include <string>
#include "token.h"
#include "runtime_error.h"

class Environment
{
private:
    /* data */
    std::unordered_map<std::string, std::any> values; // Store variable names and their values
public:
    Environment(/* args */) {}
    ~Environment() {}
    void define(const std::string &name, const std::any &value)
    {
        values[name] = value; // Define a variable with its value
    }
    void assign(const Token &token, const std::any &value)
    {
        auto it = values.find(token.get_lexeme()); // Find the variable by its name
        if (it != values.end())
        {
            it->second = value; // Update the value of the variable
        }
        else
        {
            throw RuntimeError(token, "Undefined variable '" + token.get_lexeme() + "'."); // Throw an error if the variable is not defined
        }
    }
    std::any get(const Token &token) const
    {
        auto it = values.find(token.get_lexeme()); // Find the variable by its name
        if (it != values.end())
        {
            return it->second; // Return the value of the variable
        }
        throw RuntimeError(token, "Undefined variable '" + token.get_lexeme() + "'."); // Throw an error if the variable is not defined
    }
};

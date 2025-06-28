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
    std::shared_ptr<Environment> enclosing;           // Pointer to the enclosing environment for nested scopes
public:
    Environment(std::shared_ptr<Environment> enclosing = nullptr) : enclosing(std::move(enclosing)) {}
    ~Environment() {}
    std::shared_ptr<Environment> get_enclosing() { return enclosing; }
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
            return;             // Return if the variable was found and updated
        }
        if (enclosing) // If there is an enclosing environment
        {
            enclosing->assign(token, value); // Try to assign in the enclosing environment
            return;
        }
        throw RuntimeError(token, "Undefined variable '" + token.get_lexeme() + "'."); // Throw an error if the variable is not defined
    }
    std::any get(const Token &token) const
    {
        auto it = values.find(token.get_lexeme()); // Find the variable by its name
        if (it != values.end())
        {
            return it->second; // Return the value of the variable
        }
        if (enclosing)
        {
            return enclosing->get(token); // Try to get the variable from the enclosing environment
        }
        throw RuntimeError(token, "Undefined variable '" + token.get_lexeme() + "'."); // Throw an error if the variable is not defined
    }

    int deepth()
    {
        int deep = 0;
        std::shared_ptr<Environment> ptr = enclosing;
        while (ptr.get())
        {
            deep++;
            ptr = ptr->enclosing;
        }
        return deep;
    }

    Environment *ancestor(int distance)
    {
        Environment *environment = this;
        for (int i = 0; i < distance; i++)
        {
            if (environment != nullptr)
            {
                environment = environment->enclosing.get();
            }
            else
            {
                std::cout << "env is null" << std::endl;
            }
        }
        return environment;
    }

    std::any get_at(int distance, std::string name)
    {
        return ancestor(distance)->values.at(name);
    }

    void assign_at(int distance, Token name, std::any value)
    {
        ancestor(distance)->values[name.get_lexeme()] = value;
    }
};
#pragma once
#include "callable.h"
#include <memory>

class Function : public Callable
{
private:
    /* data */
    const Stmt::Func &declaration;
    Environment enclosing;

public:
    Function(const Stmt::Func &declaration, Environment enclosing) : declaration(declaration), enclosing(enclosing) {}
    ~Function() {}
    int arity() override
    {
        return declaration.get_params().size();
    }

    std::any call(Interpreter &interpreter, std::vector<std::any> arguments) override
    {
        Environment env(&enclosing);
        std::vector<Token> params = declaration.get_params();
        for (int i = 0; i < arguments.size(); i++)
        {
            env.define(params.at(i).get_lexeme(), arguments.at(i));
        }

        try
        {
            interpreter.execute_block(declaration.get_body(), env);
            return std::any();
        }
        catch (const Return &return_stmt)
        {
            return return_stmt.get_value();
        }
    }
};

#pragma once
#include <vector>
#include "interpreter.h"

class Callable
{
public:
    virtual ~Callable() = default;
    virtual int arity() = 0;
    virtual std::any call(Interpreter &interpreter, std::vector<std::any> arguments) = 0;
    virtual std::string to_string() const = 0;
    friend std::ostream &operator<<(std::ostream &os, const Callable &callable)
    {
        os << "<fn ";
        os << callable.to_string();
        os << " >";
        return os;
    }
};
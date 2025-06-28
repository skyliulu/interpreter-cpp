#pragma once
#include <vector>
#include "interpreter.h"

class Callable
{
public:
    virtual ~Callable() = default;
    virtual int arity() = 0;
    virtual std::any call(const std::shared_ptr<Callable>& callable, Interpreter &interpreter, std::vector<std::any> arguments) = 0;
    virtual std::string to_string() const = 0;
};
#pragma once
#include "callable.h"

class Clock : public Callable
{
private:
    /* data */
public:
    Clock(/* args */) {}
    ~Clock() {}
    int arity()
    {
        return 0;
    }
    std::any call(const Interpreter &interpreter, std::vector<std::any> arguments)
    {
        return std::clock();
    }
};

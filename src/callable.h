#include <vector>
#include "interpreter.h"

class Callable
{
public:
    virtual ~Callable() = default;
    virtual int arity() = 0;
    virtual std::any call(const Interpreter &interpreter, std::vector<std::any> arguments) = 0;
};
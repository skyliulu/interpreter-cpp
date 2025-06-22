#include "lox.h"
#include <iostream>

bool Lox::had_error = false;

Lox::Lox(/* args */) {}
Lox::~Lox() {}

void Lox::run(const std::string &source)
{
    // Implementation of the run method
}

void Lox::run_file(const std::string &filename)
{
    // Implementation of the run_file method
}

void Lox::run_prompt()
{
    // Implementation of the run_prompt method
}

void Lox::report(int line, const std::string &where, const std::string &message)
{
    std::cerr << "[line " << line << "] Error" << where << ": " << message << std::endl;
}

void Lox::error(int line, const std::string &message)
{
    report(line, "", message);
    had_error = true;
}
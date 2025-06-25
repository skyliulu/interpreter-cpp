#include "lox.h"
#include "scanner.h"
#include <iostream>

bool Lox::had_error = false;

Lox::Lox(/* args */) {}
Lox::~Lox() {}

void Lox::tokenize(const std::string &source)
{
    // Implementation of the run method
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scan_tokens();
    for (const Token &token : tokens)
    {
        std::cout << token << std::endl;
    }
    if (had_error)
    {
        exit(65); // Exit with an error code if there was an error
    }
    return;
}

void Lox::parse(const std::string &source)
{
    // Implementation of the parse method
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scan_tokens();
    // for (const Token &token : tokens)
    // {
    //     std::cout << token << std::endl;
    // }
    if (had_error)
    {
        exit(65); // Exit with an error code if there was an error
        return;
    }

    Parser parser(tokens);
    std::vector<std::unique_ptr<Expr>> expressions = parser.parse();
    if (had_error)
    {
        // std::cerr << "Parsing failed with errors." << std::endl;
        exit(65); // Exit with an error code if there was an error
        return;
    }
    AstPrinter printer;
    for (const auto &expr : expressions)
    {
        expr->accept(printer);
        std::cout << std::endl;
    }
}

void Lox::evaluate(const std::string &source)
{
    // Implementation of the evaluate method
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scan_tokens();
    if (had_error)
    {
        exit(65); // Exit with an error code if there was an error
        return;
    }

    Parser parser(tokens);
    std::vector<std::unique_ptr<Expr>> expressions = parser.parse();
    if (had_error)
    {
        exit(65); // Exit with an error code if there was an error
        return;
    }
    Interpreter interpreter;
    for (const auto &expr : expressions)
    {
        interpreter.interpret(*expr);
    }
    if (had_error)
    {
        exit(65); // Exit with an error code if there was an error
        return;
    }
}

void Lox::run(const std::string &source)
{
    // Implementation of the run method
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scan_tokens();
    for (const Token &token : tokens)
    {
        std::cout << token << std::endl;
    }
    if (had_error)
    {
        exit(65); // Exit with an error code if there was an error
    }
    return;
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

void Lox::error(const Token &token, const std::string &message)
{
    if (token.get_type() == TokenType::EOF_TOKEN)
    {
        report(token.get_line(), " at end", message);
    }
    else
    {
        report(token.get_line(), " at '" + token.get_lexeme() + "'", message);
    }
    had_error = true;
}
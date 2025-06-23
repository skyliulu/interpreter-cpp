#include <vector>
#include <string>
#include <stdexcept>
#include <memory>
#include "expr.h"

// Define a custom exception class for parser errors
class ParserError : public std::runtime_error
{
public:
    explicit ParserError() : std::runtime_error("")
    {
    }

    // Constructor that takes an error message
    explicit ParserError(const std::string &message)
        : std::runtime_error(message)
    {
        // The message is stored internally by std::runtime_error
    }
};

class Parser
{
private:
    /* data */
    std::vector<Token> tokens; // Placeholder for tokens, replace with actual token type
    int current;               // Current position in the token list
    /* parse expr */
    std::unique_ptr<Expr> expresstion();
    std::unique_ptr<Expr> primary();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> binary();
    std::unique_ptr<Expr> grouping();
    /* helper functions */
    bool is_at_end();
    bool check(TokenType type);
    bool check_next(TokenType type);
    Token peek();
    Token previous();
    Token advance();
    Token consume(TokenType type, const std::string &message);
    bool match(const std::vector<TokenType> &types);
    ParserError error(Token token, const std::string &message);

public:
    Parser(const std::vector<Token> &tokens);
    ~Parser();
    std::vector<std::unique_ptr<Expr>> parse();
};

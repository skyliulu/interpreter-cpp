#include <string>
#include <optional>
#include <variant>

enum TokenType
{
    // Single-character tokens.
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    SLASH,
    STAR,

    // One or two character tokens.
    BANG,
    BANG_EQUAL,
    EQUAL,
    EQUAL_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,

    // Literals.
    IDENTIFIER,
    STRING,
    NUMBER,

    // Keywords.
    AND,
    CLASS,
    ELSE,
    FALSE,
    FUN,
    FOR,
    IF,
    NIL,
    OR,
    PRINT,
    RETURN,
    SUPER,
    THIS,
    TRUE,
    VAR,
    WHILE,

    EOF_TOKEN, // End of file token
};

using Literal = std::variant<std::string, double, bool, std::nullptr_t>;

class Token
{
private:
    /* data */
    const TokenType type;
    const std::string lexeme;
    const std::optional<Literal> literal;
    const int line;

public:
    Token(TokenType type, const std::string &lexeme, const std::optional<Literal> literal, int line);
    ~Token();
    TokenType get_type() const;
    std::string get_lexeme() const;
    const std::optional<Literal> &get_literal() const;
    int get_line() const;
    friend std::ostream &operator<<(std::ostream &os, const Token &token);
};

std::string TokentypeToString(TokenType type);
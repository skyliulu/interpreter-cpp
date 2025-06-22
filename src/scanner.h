#include "token.h"
#include <vector>

class Scanner
{
private:
    /* data */
    const std::string source;  // The source code to scan
    std::vector<Token> tokens; // The list of tokens generated from the source code
    int start = 0;             // Start index of the current token
    int current = 0;           // Current index in the source code
    int line = 1;              // Current line number in the source code
    // Helper methods for scanning
    void scan_token();
    void add_token(Tokentype type, const std::optional<std::string> literal = std::nullopt);
    bool is_at_end() const;
    char advance();
    char peek() const;
    char peek_next() const;
    bool match(char expected);

public:
    Scanner(const std::string &source);
    ~Scanner();
    std::vector<Token> scan_tokens();
};
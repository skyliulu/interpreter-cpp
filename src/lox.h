#include <string>

class Lox
{
private:
    /* data */
    static bool had_error; // Flag to indicate if an error has occurred
public:
    Lox(/* args */);
    ~Lox();
    static void run(const std::string &source); // Method to run the Lox interpreter with the given source code
    static void run_file(const std::string &filename); // Method to run a Lox file
    static void run_prompt(); // Method to run the Lox interpreter in prompt mode
    static void report(int line, const std::string &where, const std::string &message); // Method to report errors
    static void error(int line, const std::string &message); // Method to handle errors
};
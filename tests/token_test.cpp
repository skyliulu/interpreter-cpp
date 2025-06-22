#include <gtest/gtest.h>
#include "token.h"

TEST(TokentypeToStringTest, BasicTokens) {
    EXPECT_EQ(TokentypeToString(LEFT_PAREN), "LEFT_PAREN");
    EXPECT_EQ(TokentypeToString(RIGHT_PAREN), "RIGHT_PAREN");
    EXPECT_EQ(TokentypeToString(PLUS), "PLUS");
    EXPECT_EQ(TokentypeToString(EOF_TOKEN), "EOF_TOKEN");
}

TEST(TokentypeToStringTest, UnknownToken) {
    EXPECT_EQ(TokentypeToString(static_cast<Tokentype>(9999)), "UNKNOWN");
}
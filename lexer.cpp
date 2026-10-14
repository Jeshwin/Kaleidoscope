#include "lexer.hpp"

#include <stdio.h>
#include <stdlib.h>

#include <string>

enum Token {
    token_eof = -1,
    token_def = -2,
    token_extern = -3,
    token_identifier = -4,
    token_number = -5,
};

static std::string IdentifierStr;
static double NumberValue;

// Get next token from stdin
static int gettok() {
    static int LastChar = ' ';

    // Trim leading whitespace
    while (isspace(LastChar)) LastChar = getchar();

    // Read a word
    // isalpha matches regex [a-zA-Z][a-zA-Z0-9]*
    if (isalpha(LastChar)) {
        IdentifierStr = LastChar;
        while (isalnum((LastChar = getchar()))) IdentifierStr += LastChar;

        // Check keywords
        if (IdentifierStr == "def") return token_def;
        if (IdentifierStr == "extern") return token_extern;

        return token_identifier;
    }

    // Read a number
    // Matches regex [0-9.]+
    if (isdigit(LastChar) || LastChar == '.') {
        std::string NumberStr;
        do {
            NumberStr += LastChar;
            LastChar = getchar();
        } while (isdigit(LastChar) || LastChar == '.');

        NumberValue = strtod(NumberStr.c_str(), 0);
        return token_number;
    }

    // Handle comments `#`
    if (LastChar == '#') {
        // Comment until end of line.
        do LastChar = getchar();
        while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if (LastChar != EOF) return gettok();
    }

    // Check for end of file.  Don't eat the EOF.
    if (LastChar == EOF) return token_eof;

    // Otherwise, just return the character as its ascii value.
    int ThisChar = LastChar;
    LastChar = getchar();
    return ThisChar;
}
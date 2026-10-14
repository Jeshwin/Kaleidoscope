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

bool is_valid_id_char(int c) { return isalnum(c) || c == '_'; }

// Handle floating point number with leading or trailing decimal point
double get_num_token(int LastChar) {
    bool has_decimal = false;
    std::string NumberStr;
    do {
        NumberStr += LastChar;
        if (!has_decimal && LastChar == '.') has_decimal = true;
        LastChar = getchar();
    } while (isdigit(LastChar) || (LastChar == '.' && !has_decimal));

    return strtod(NumberStr.c_str(), 0);
}

// Get next token from stdin
static int get_token() {
    static int LastChar = ' ';

    // Trim whitespace
    while (isspace(LastChar)) LastChar = getchar();

    // Read a word
    if (isalpha(LastChar)) {
        IdentifierStr = LastChar;
        while (is_valid_id_char((LastChar = getchar())))
            IdentifierStr += LastChar;

        // Check keywords
        if (IdentifierStr == "def") return token_def;
        if (IdentifierStr == "extern") return token_extern;

        return token_identifier;
    }

    // Read a floating point number
    if (isdigit(LastChar) || LastChar == '.') {
        NumberValue = get_num_token(LastChar) return token_number;
    }

    // Handle comments `#`
    if (LastChar == '#') {
        // Comment until end of line.
        do LastChar = getchar();
        while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if (LastChar != EOF) return get_token();
    }

    // Check for end of file.  Don't eat the EOF.
    if (LastChar == EOF) return token_eof;

    // Otherwise, just return the character as its ascii value.
    int ThisChar = LastChar;
    LastChar = getchar();
    return ThisChar;
}
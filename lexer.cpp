#include "lexer.hpp"

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

enum Token {
    token_eof = -1,
    token_fn = -2,
    token_extern = -3,
    token_identifier = -4,
    token_number = -5,
};

static std::string IdentifierStr;
static double NumberValue;

bool is_valid_id_char(int c) { return isalnum(c) || c == '_'; }

// Handle words
std::string get_word_token(int& LastChar, std::istream& in) {
    std::string s;
    s = LastChar;
    while (is_valid_id_char((LastChar = in.get()))) s += LastChar;
    return s;
}

// Handle floating point number with leading or trailing decimal point
double get_num_token(int& LastChar, std::istream& in) {
    bool has_decimal = false;
    std::string NumberStr;
    do {
        NumberStr += LastChar;
        if (!has_decimal && LastChar == '.') has_decimal = true;
        LastChar = in.get();
    } while (isdigit(LastChar) || (LastChar == '.' && !has_decimal));

    return strtod(NumberStr.c_str(), 0);
}

// Get next token from stdin
static int get_token(std::istream& in = std::cin) {
    static int LastChar = ' ';

    // Trim whitespace
    while (isspace(LastChar)) LastChar = in.get();

    // Read a word
    if (isalpha(LastChar)) {
        IdentifierStr = get_word_token(LastChar, in);

        // Check keywords
        if (IdentifierStr == "fn") return token_fn;
        if (IdentifierStr == "extern") return token_extern;

        return token_identifier;
    }

    // Read a floating point number
    if (isdigit(LastChar) || LastChar == '.') {
        NumberValue = get_num_token(LastChar, in);
        return token_number;
    }

    // Handle comments `#`
    if (LastChar == '#') {
        // Comment until end of line.
        do LastChar = in.get();
        while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if (LastChar != EOF) return get_token(in);
    }

    // Check for end of file.  Don't eat the EOF.
    if (LastChar == EOF) return token_eof;

    // Otherwise, just return the character as its ascii value.
    int ThisChar = LastChar;
    LastChar = in.get();
    return ThisChar;
}
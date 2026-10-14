#include "lexer.hpp"

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

#include "util/pretty_print.hpp"

// Helper function to test a number input
static double lex_num(const char* s, int& next) {
    std::istringstream in(s);
    next = in.get();
    return get_num_token(next, in);
}

// Test number lexer
void test_get_num_token() {
    static int c = ' ';
    assert(lex_num("1", c) == 1.0 && c == EOF);
    assert(lex_num("10023;", c) == 10023.0 && c == ';');
    assert(lex_num("10.", c) == 10.0 && c == EOF);
    assert(lex_num("1.25", c) == 1.25 && c == EOF);
    assert(lex_num(".1", c) == 0.1 && c == EOF);
}

// Helper function to test a word input
static std::string lex_word(const char* s, int& next) {
    std::istringstream in(s);
    next = in.get();
    return get_word_token(next, in);
}

// Test number lexer
void test_get_word_token() {
    static int c = ' ';
    assert(lex_word("fn", c) == "fn" && c == EOF);
    assert(lex_word("word;", c) == "word" && c == ';');
    assert(lex_word("snake_case", c) == "snake_case" && c == EOF);
    assert(lex_word("dots.bad", c) == "dots" && c == '.');
    assert(lex_word("x, y", c) == "x" && c == ',');
}

int main() {
    test_get_num_token();
    test_get_word_token();
    pretty_print("All lexer test passed!", FGColor::green);
    return 0;
}

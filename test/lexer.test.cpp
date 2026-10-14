#include "lexer.hpp"

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

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

// Helper function to test a comment input
static bool lex_comment(const char* s, int& next) {
    std::istringstream in(s);
    next = in.get();
    return skip_comment(next, in);
}

// Test number lexer
void test_skip_comment() {
    static int c = ' ';
    assert(lex_comment("// Hello\nx", c) && c == '\n');
    assert(lex_comment("// eof", c) && c == EOF);
    assert(lex_comment("/* x\n*/y", c) && c == 'y');
    assert(lex_comment("/**/x", c) && c == 'x');
    assert(lex_comment("/* a ** b */x", c) && c == 'x');
    assert(lex_comment("/* a / b */x", c) && c == 'x');
    assert(!lex_comment("/ Not a comment", c) && c == '/');
    assert(lex_comment("/* Forgot to close", c) && c == EOF);
}

int main() {
    test_get_num_token();
    test_get_word_token();
    test_skip_comment();
    std::cout << "\033[32mAll lexer tests passed!\033[0m\n";  // green
    return 0;
}

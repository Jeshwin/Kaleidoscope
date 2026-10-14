#include "lexer.hpp"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <string>

bool test_gettok() {
    // Test number parsing
    assert(gettok("1"));
    assert(gettok("10"));
    assert(gettok("1.25"));
    assert(gettok("100."));
    assert(gettok(".67"));
}

int main() {
    test_gettok();
    return 0;
}

#pragma once

#include <iostream>
#include <string>

enum FGColor {
    black = 30,
    red,    // = 31
    green,  // = 32 ...
    yellow,
    blue,
    magenta,
    cyan,
    white,
    bright_black = 90,
    bright_red,    // = 91
    bright_green,  // = 92 ...
    bright_yellow,
    bright_blue,
    bright_magenta,
    bright_cyan,
    bright_white,
};

void pretty_print(const std::string& s, FGColor fgcolor);
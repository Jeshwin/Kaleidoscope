#include "pretty_print.hpp"

#include <iostream>
#include <string>

void pretty_print(const std::string& s, FGColor fgcolor) {
    std::cout << "\033[" << fgcolor << "m" << s << "\033[0m" << std::endl;
}
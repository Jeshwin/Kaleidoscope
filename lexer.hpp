#pragma once
#include <iostream>
#include <string>

std::string get_word_token(int& LastChar, std::istream& in);
double get_num_token(int& LastChar, std::istream& in);
bool skip_comment(int& LastChar, std::istream& in);

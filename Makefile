CXX      := clang++
CXXFLAGS := -std=c++17 -Wall -Wextra -g
CPPFLAGS := -MMD -MP            # auto header dependency tracking (.d files)
BUILD    := build

LEXER_OBJ  := $(BUILD)/lexer.o
PARSER_OBJ := $(BUILD)/parser.o

.PHONY: all lexer parser clean
all: lexer parser

lexer:  $(LEXER_OBJ)
parser: $(PARSER_OBJ)

# Future driver: links lexer + parser into one executable
# DRIVER := $(BUILD)/kaleidoscope
# $(DRIVER): $(BUILD)/driver.o $(LEXER_OBJ) $(PARSER_OBJ)
# 	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD)/%.o: %.cpp | $(BUILD)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(BUILD):
	mkdir -p $@

clean:
	rm -rf $(BUILD)

-include $(wildcard $(BUILD)/*.d)

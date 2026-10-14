CXX      := clang++
CXXFLAGS := -std=c++17 -Wall -Wextra -g
CPPFLAGS := -MMD -MP # auto header dependency tracking (.d files)
BUILD    := build

LEXER_OBJ  := $(BUILD)/lexer.o
PARSER_OBJ := $(BUILD)/parser.o

.PHONY: all lexer parser test clean
all: lexer parser

lexer:  $(LEXER_OBJ)
parser: $(PARSER_OBJ)

# Every top-level and util/ .cpp is linked into tests; driver.cpp has its own main() so it's excluded
SRCS  := $(filter-out driver.cpp,$(wildcard *.cpp util/*.cpp))
OBJS  := $(SRCS:%.cpp=$(BUILD)/%.o)
TESTS := $(patsubst test/%.test.cpp,$(BUILD)/test/%,$(wildcard test/*.test.cpp))

test: $(TESTS)
	@for t in $^; do echo "== $$t"; ./$$t || exit 1; done

# Link each test against all modules, since a parser test needs the lexer, etc.
$(BUILD)/test/%: test/%.test.cpp $(OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) -I. $(CXXFLAGS) $(filter %.cpp %.o,$^) -o $@

# Future driver: links lexer + parser into one executable
# DRIVER := $(BUILD)/kaleidoscope
# $(DRIVER): $(BUILD)/driver.o $(LEXER_OBJ) $(PARSER_OBJ)
# 	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) -I. $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD)

-include $(wildcard $(BUILD)/*.d $(BUILD)/*/*.d)

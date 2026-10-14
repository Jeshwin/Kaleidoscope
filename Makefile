CXX      := clang++
CXXFLAGS := -std=c++17 -Wall -Wextra -g
CPPFLAGS := -MMD -MP # auto header dependency tracking (.d files)
BUILD    := build

LEXER_OBJ  := $(BUILD)/lexer.o
PARSER_OBJ := $(BUILD)/parser.o

.PHONY: all lexer parser test coverage clean
all: lexer parser

lexer:  $(LEXER_OBJ)
parser: $(PARSER_OBJ)

# Every top-level .cpp is linked into tests; driver.cpp has its own main() so it's excluded
SRCS  := $(filter-out driver.cpp,$(wildcard *.cpp))
OBJS  := $(SRCS:%.cpp=$(BUILD)/%.o)
TESTS := $(patsubst test/%.test.cpp,$(BUILD)/test/%,$(wildcard test/*.test.cpp))

test: $(TESTS)
	@for t in $^; do echo "== $$t"; ./$$t || exit 1; done

# Link each test against all modules, since a parser test needs the lexer, etc.
$(BUILD)/test/%: test/%.test.cpp $(OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) -I. $(CXXFLAGS) $(filter %.cpp %.o,$^) -o $@

# Instrumented build in its own dir so normal builds stay uninstrumented
COV     := $(BUILD)/cov
COV_MIN := 80

coverage:
	@find $(COV) -name '*.gcda' -delete 2>/dev/null || true
	@$(MAKE) --no-print-directory BUILD=$(COV) CXXFLAGS="$(CXXFLAGS) --coverage" test
	@cd $(COV) && for s in $(SRCS); do gcov -o $${s%.cpp}.o $(CURDIR)/$$s; done 2>/dev/null | awk -v min=$(COV_MIN) ' \
		/^File / { f = $$2; gsub("\047", "", f); ours = f !~ /^\// } \
		ours && /^Lines executed/ { split($$2, a, /[:%]/); pct = a[2]; n++; \
			printf "%-28s %6.2f%%%s\n", f, pct, (pct < min ? "  < $(COV_MIN)%" : ""); if (pct < min) bad = 1 } \
		END { if (!n) { print "no coverage data"; exit 1 } if (bad) { print "coverage below $(COV_MIN)%; see $(COV)/*.gcov"; exit 1 } }'

# Future driver: links lexer + parser into one executable
# DRIVER := $(BUILD)/kaleidoscope
# $(DRIVER): $(BUILD)/driver.o $(LEXER_OBJ) $(PARSER_OBJ)
# 	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) -I. $(CXXFLAGS) -c $< -o $@

# Keep objects make would otherwise treat as intermediates and delete
.SECONDARY:

clean:
	rm -rf $(BUILD)

-include $(wildcard $(BUILD)/*.d $(BUILD)/*/*.d)

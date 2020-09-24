BIN := main

CXXFLAGS := -std=c++17 -Wconversion -Wall -Werror -Wextra -pedantic -DNDEBUG -O3

DEBUG := -std=c++17 -Wconversion -Wall -Werror -Wextra -pedantic -DDEBUG -g3

SRCS := $(shell find src  -name "*.cpp")

all: $(BIN)

$(BIN): $(SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $@

debug: $(SRCS)
	$(CXX) $(DEBUG) $^ -o $(BIN)_$@

.PHONY: clean all
clean:
	rm -rvf *.out *.exe main main_debug

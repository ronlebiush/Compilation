# Variables
LEX = flex
CXX = g++
CXXFLAGS = -std=c++17
LEX_FILE = scanner.lex
LEX_OUTPUT = lex.yy.c
SRC = hw1.cpp
TARGET = hw1.out

# Default target
all: $(TARGET)

# Rule to build the final executable
$(TARGET): $(LEX_OUTPUT) $(SRC)
	$(CXX) $(CXXFLAGS) $(LEX_OUTPUT) $(SRC) -o $(TARGET)

# Rule to generate the lex.yy.c file from the .lex file
$(LEX_OUTPUT): $(LEX_FILE)
	$(LEX) $(LEX_FILE)

# Clean up generated files
clean:
	rm -f $(LEX_OUTPUT) $(TARGET)

.PHONY: all clean

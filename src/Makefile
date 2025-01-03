# Compiler and flags
CXX = g++ -std=c++11 
CXXFLAGS = $(shell pkg-config --cflags sdl2)
LDFLAGS = $(shell pkg-config --libs sdl2)

# Output executable
TARGET = shell

# Source files
SRC = main.cpp 

# Default target
all: $(TARGET)

# Compile and link the program
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

# Clean up compiled files
clean:
	rm -f $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)
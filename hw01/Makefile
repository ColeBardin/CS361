SRC_DIR := src
SRC := $(wildcard $(SRC_DIR)/*.cpp)
INC_DIR := inc
BIN_DIR := bin

CC := g++
CCFLAGS := -std=c++17
CXXFLAGS := -I$(INC_DIR)

all: directories

directories:
	@mkdir -p $(BIN_DIR)

monty:
	$(CC) -o $(BIN_DIR)/monty $(SRC) $(CCFLAGS) $(CXXFLAGS)

clean:
	rm -f $(BIN_DIR)/*

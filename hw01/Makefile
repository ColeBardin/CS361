SRC_DIR := src
SRC := $(wildcard $(SRC_DIR)/*.cpp)
INC_DIR := inc
BIN_DIR := bin

CC := g++
CCFLAGS := -std=c++17
CXXFLAGS := -I$(INC_DIR)

DIR_CHK = @mkdir -p $(BIN_DIR)

monty:
	$(DIR_CHK)
	$(CC) -o $(BIN_DIR)/monty $(SRC) $(CCFLAGS) $(CXXFLAGS)

clean:
	rm -f $(BIN_DIR)/*

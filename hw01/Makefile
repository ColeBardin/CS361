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
none:
	$(BIN_DIR)/monty
cat:
	$(BIN_DIR)/monty cat
test16:
	$(BIN_DIR)/monty 16
test100:
	$(BIN_DIR)/monty 100
test2000:
	$(BIN_DIR)/monty 2000
test10000:
	$(BIN_DIR)/monty 10000
clean:
	rm -f $(BIN_DIR)/*

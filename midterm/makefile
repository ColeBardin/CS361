STD=--std=c++17 -pthread
GCC=g++
OBJ=obj
BIN=bin
EXE=threadpuzzle

bin/${EXE}: src/main.cpp obj/semaphore.o
	[ -d $(BIN) ] || mkdir -p $(BIN)
	${GCC} ${STD} -o bin/${EXE} src/main.cpp obj/semaphore.o

obj/semaphore.o: src/semaphore.h src/semaphore.cpp
	[ -d $(OBJ) ] || mkdir -p $(OBJ)
	${GCC} ${STD} -c -o obj/semaphore.o src/semaphore.cpp

test: bin/${EXE}
	bin/${EXE} 4 12 2

.PHONY: doc
doc:
	doxygen config
	cd latex && make

clean:
	rm -f bin/run
	rm -r -f bin
	rm -r -f obj
	rm -r -f html
	rm -r -f latex

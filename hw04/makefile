STD=--std=c++17 -pthread
GCC=g++
OBJ=obj
BIN=bin

bin/race: src/main.cpp obj/semaphore.o
	[ -d $(BIN) ] || mkdir -p $(BIN)
	${GCC} ${STD} -o bin/race src/main.cpp obj/semaphore.o

obj/semaphore.o: src/semaphore.h src/semaphore.cpp
	[ -d $(OBJ) ] || mkdir -p $(OBJ)
	${GCC} ${STD} -c -o obj/semaphore.o src/semaphore.cpp

run: bin/race
	bin/race 2 || true
	bin/race 4 || true
	bin/race 10 || true

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

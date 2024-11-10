CC=g++
STD=--std=c++17 -pthread

SRC=src
OBJ=obj
BIN=bin
EXE=bar

${EXE}: ${SRC}/main.cpp ${OBJ}/semaphore.o 
	[ -d $(BIN) ] || mkdir -p $(BIN)
	${CC} ${STD} -o ${BIN}/${EXE} ${SRC}/main.cpp ${OBJ}/semaphore.o

${OBJ}/semaphore.o: ${SRC}/semaphore.h ${SRC}/semaphore.cpp 
	[ -d $(OBJ) ] || mkdir -p $(OBJ)
	${CC} ${CFLAGS} -c -o ${OBJ}/semaphore.o ${SRC}/semaphore.cpp

test1: ${EXE}
	${BIN}/${EXE} 10 || true

.PHONY: doc
doc:
	doxygen config
	#cd latex && make

clean:
	rm -f bin/run
	rm -r -f bin
	rm -r -f obj
	rm -r -f html
	rm -r -f latex

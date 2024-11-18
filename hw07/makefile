CC=g++
STD=--std=c++17 -pthread

SRC=src
OBJ=obj
BIN=bin
EXE=island

${EXE}: ${SRC}/main.cpp ${OBJ}/semaphore.o
	[ -d $(BIN) ] || mkdir -p $(BIN)
	${CC} ${STD} -o ${BIN}/${EXE} ${SRC}/main.cpp ${OBJ}/semaphore.o

${OBJ}/semaphore.o: ${SRC}/semaphore.h ${SRC}/semaphore.cpp 
	[ -d $(OBJ) ] || mkdir -p $(OBJ)
	${CC} ${CFLAGS} -c -o ${OBJ}/semaphore.o ${SRC}/semaphore.cpp

run: ${EXE}
	${BIN}/${EXE} 7 9 || true

.PHONY: doc
doc:
	doxygen config
	cd latex && make

clean:
	rm -f ${BIN}/${EXE}
	rm -r -f bin
	rm -r -f obj
	rm -r -f html
	rm -r -f latex

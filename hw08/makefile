CC=g++
STD=--std=c++17 -pthread

SRC=src
OBJ=obj
BIN=bin
EXE=search

${EXE}: ${SRC}/main.cpp ${OBJ}/threadPool.o ${OBJ}/searchTask.o
	[ -d $(BIN) ] || mkdir -p $(BIN)
	${CC} ${STD} -o ${BIN}/${EXE} ${SRC}/main.cpp ${OBJ}/threadPool.o ${OBJ}/searchTask.o

${OBJ}/searchTask.o: ${SRC}/task.h ${SRC}/searchTask.h ${SRC}/searchTask.cpp ${SRC}/channel.h
	mkdir -p obj
	${CC} ${CFLAGS} -c -o ${OBJ}/searchTask.o ${SRC}/searchTask.cpp

${OBJ}/threadPool.o: ${SRC}/task.h ${SRC}/threadPool.h ${SRC}/threadPool.cpp
	mkdir -p obj
	${CC} ${CFLAGS} -c -o ${OBJ}/threadPool.o ${SRC}/threadPool.cpp

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

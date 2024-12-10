CC=g++
STD=--std=c++17 -pthread

SRC=src
OBJ=obj
BIN=bin
EXE=safari

${EXE}: ${SRC}/main.cpp ${SRC}/channel.h
	[ -d $(BIN) ] || mkdir -p $(BIN)
	${CC} ${STD} -o ${BIN}/${EXE} ${SRC}/main.cpp 

run: ${EXE}
	${BIN}/${EXE} 16 3 || true

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

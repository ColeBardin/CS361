CC=g++
STD=--std=c++17 -pthread

SRC=src
OBJ=obj
BIN=bin
EXE=mandelbrot

${EXE}: ${SRC}/main.cpp ${OBJ}/bmp.o ${SRC}/pixel.h ${SRC}/bmp.h 
	[ -d $(BIN) ] || mkdir -p $(BIN)
	${CC} ${STD} -o ${BIN}/${EXE} ${SRC}/main.cpp ${OBJ}/bmp.o

${OBJ}/bmp.o: ${SRC}/bmp.h ${SRC}/pixel.h ${SRC}/bmp.cpp
	[ -d $(OBJ) ] || mkdir -p $(OBJ)
	${CC} ${CFLAGS} -c -o ${OBJ}/bmp.o ${SRC}/bmp.cpp

e0.bmp: ${EXE}
	${BIN}/${EXE} -2 1 -1.5 1.5 e0.bmp || true

e1.bmp: ${EXE}
	${BIN}/${EXE} -0.5 0 0.3 1.2 e1.bmp || true

e2.bmp: ${EXE}
	${BIN}/${EXE} 0.3 0.4 0.6 0.7 e2.bmp || true

e3.bmp: ${EXE}
	${BIN}/${EXE} -0.2 0.0 -1.0 -0.9 e3.bmp || true

e4.bmp: ${EXE}
	${BIN}/${EXE} -0.05 -0.01 -1.01 -0.97 e4.bmp || true

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

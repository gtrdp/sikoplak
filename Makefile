all: main

release: ai.o io.o grid.o main.o 
		g++ --std=c++11 -O3 -static main.o ai.o io.o grid.o -o2048 -lgdi32

main: ai.o io.o grid.o main.o 
		g++ --std=c++11 main.o ai.o io.o grid.o -o2048 -lgdi32

main.o: 2048.h 2048.cpp
		g++ --std=c++11 -c -omain.o 2048.cpp

ai.o: 2048.h 2048Grid.h 2048AI.h 2048AI.cpp
		g++ --std=c++11 -c -oai.o 2048AI.cpp

io.o: 2048.h 2048IO.h 2048IO.cpp
		g++ --std=c++11 -c -oio.o 2048IO.cpp

grid.o: 2048.h 2048Grid.h 2048Grid.cpp
		g++ --std=c++11 -c -ogrid.o 2048Grid.cpp

clean:
		rm -rf *.o
		rm -rf 2048.exe
		
		
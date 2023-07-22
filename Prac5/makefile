prac5.out: main.o node.o table.o trans.o
	g++ main.o node.o table.o trans.o -o prac5.out

main.o: main.cpp
	g++ -c main.cpp

node.o: node.cpp
	g++ -c node.cpp

table.o: table.cpp
	g++ -c table.cpp

trans.o: trans.cpp
	g++ -c trans.cpp

run:
	./prac5.out

clean:
	rm*.o prac5




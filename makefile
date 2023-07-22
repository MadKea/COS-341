prac1.out: main.o NFA.o node.o transition.o DFA.o minDFA.o 
	g++ main.o NFA.o node.o transition.o DFA.o minDFA.o  -o prac1.out

main.o: main.cpp
	g++ -c main.cpp

NFA.o: NFA.cpp
	g++ -c NFA.cpp

DFA.o: DFA.cpp
	g++ -c DFA.cpp

minDFA.o: minDFA.cpp
	g++ -c minDFA.cpp
	
node.o: node.cpp
	g++ -c node.cpp

transition.o: transition.cpp
	g++ -c transition.cpp

run:
	./prac1.out


clean:
	rm*.o prac1


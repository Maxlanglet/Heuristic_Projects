program : src/pfspinstance.h src/flowshop.cpp src/pfspinstance.cpp src/Timer.hpp

	g++ -O3 -c ./src/flowshop.cpp -o src/flowshop.o
	g++ -O3 -c ./src/pfspinstance.cpp -o src/pfspinstance.o
	g++ -O3 -c ./src/Timer.cpp -o src/Timer.o

	g++ -O3 src/flowshop.o src/pfspinstance.o src/Timer.o -o flowshopWCT

clean:
	rm src/*.o flowshopCWT

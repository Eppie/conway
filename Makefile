assembly=-fverbose-asm -Wa,-alhnd
all:
	gcc -std=c++11 -Wall -Ofast -pg -g -p -mpopcnt -funroll-loops conway.cpp -o conway $(assembly) > conway.s
	g++ -std=c++11 -Wall -Ofast -pg -g -p evogcc.cpp -o evogcc

clean:
	rm -f conway evogcc gmon.out callgrind.out* *.gcda conway.s

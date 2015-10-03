assembly=-fverbose-asm -Wa,-alhnd
all:
	gcc -std=c++11 -Wall -Ofast -g -mtune=native -march=native -funroll-loops conway.cpp -o conway $(assembly) > conway.s
	g++ -std=c++11 -Wall -Ofast -g -mtune=native -march=native evogcc.cpp -o evogcc

clean:
	rm -f conway evogcc gmon.out callgrind.out* *.gcda conway.s

profile: clean all
	valgrind -v --tool=callgrind --dump-instr=yes --dump-line=yes --collect-jumps=yes --collect-systime=yes --collect-bus=yes --cache-sim=yes --branch-sim=yes --simulate-hwpref=yes --cacheuse=yes ./conway


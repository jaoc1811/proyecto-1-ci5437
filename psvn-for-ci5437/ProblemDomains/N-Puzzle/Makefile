CC = gcc
CXX = g++
OPT = -Wall -O3 -Wno-unused-function -Wno-unused-variable -g
PSVNOPT = --history_len=0

%.idaStar: %.c ../../global/idaStar.cpp
	$(CXX) $(OPT) ../../global/idaStar.cpp ../../global/heuristics.cpp -include $< -o $@
	rm -f $*.c

%.aStar: %.c ../../global/aStar.cpp
	$(CXX) $(OPT) ../../global/aStar.cpp ../../global/node.cpp ../../global/heuristics.cpp -include $< -o $@
	rm -f $*.c

psvn2c_core.c:
	cp ../../src/psvn2c_core.c ./psvn2c_core.c

psvn2c_state_map.c:
	cp ../../src/psvn2c_state_map.c ./psvn2c_state_map.c

psvn2c_abstraction.c:
	cp ../../src/psvn2c_abstraction.c ./psvn2c_abstraction.c

%.c: %.psvn psvn2c_core.c psvn2c_state_map.c psvn2c_abstraction.c
	../../bin/psvn2c $(PSVNOPT) --name=$(*F) < $< > $@
	rm -f ./psvn2c_core.c ./psvn2c_state_map.c ./psvn2c_abstraction.c

.PRECIOUS: %.c

.PHONY: clean
clean:
	rm -fr *.idaStar psvn2c_core.c psvn2c_state_map.c psvn2c_abstraction.c *.dSYM *.o *~

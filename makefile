EXE = RedCarving
LINK = -lm


all: core.o
	gcc -Wall $^ $(LINK) -o $(EXE)
core.o: core.c core.h
	gcc -c $< $(LINK)
clear:
	rm *.o

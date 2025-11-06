#makefile
sensors: sensors.o led2472g.o hts221.o lps25h.o lsm9ds1.o makefile
		gcc -g -o sensors sensors.o led2472g.o hts221.o lps25h.o lsm9ds1.o -li2c -lcurses
sensors.o: sensors.c sensors.h
		gcc -g -c sensors.c
led24772g.o: led2472g.c led2472g.h
		gcc -g -c led2472g.c
hts221.o: hts221.c hts221.h
		gcc -g -c hts221.c
lps25h.o: lps25h.c lps25h.h
		gcc -g -c lps25h.c
lsm9ds1.o: lsm9ds1.c lsm9ds1.h
		gcc -g -c lsm9ds1.c
.PHONY: clean
clean:
		rm -f *.o
		touch *
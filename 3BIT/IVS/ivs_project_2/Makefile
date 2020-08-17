CC = g++

CFLAGS = -std=c++11 -Wall -Wextra -pedantic

PROFFLAGS = -g -pg

RM = rm -f

PROGS = ivs profiling

ALL_MODULES = math_library.o

PROF_ALL_MODULES = prof_math_library.o

all: ivs

ivs: ivs.o
	$(CC) $(CFLAGS) $(ALL_MODULES) ivs.o -o $@
	
ivs.o: $(ALL_MODULES) main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o $@

math_library.o: math_library.cpp math_library.h
	$(CC) $(CFLAGS) -c math_library.cpp -o $@

profiling: profiling.o
	$(CC) $(CFLAGS) $(PROFFLAGS) $(PROF_ALL_MODULES) profiling.o -o $@
	./profiling <profiling_sample_0010.txt
	mv gmon.out gmon.sum
	./profiling <profiling_sample_0100.txt
	gprof -s profiling gmon.out gmon.sum
	./profiling <profiling_sample_1000.txt
	gprof -s profiling gmon.out gmon.sum
	gprof profiling gmon.sum >../profiling/vystup.txt

profiling.o: $(PROF_ALL_MODULES) profiling.cpp
	$(CC) $(CFLAGS) $(PROFFLAGS) -c profiling.cpp -o $@

prof_math_library.o: math_library.cpp math_library.h
	$(CC) $(CFLAGS) $(PROFFLAGS) -c math_library.cpp -o $@


#build gui
gui:
	@cd gui-QT && qmake; make
	mv gui-QT/calculator ./calculator

run: 
	./calculator

clean:
	$(RM) *.o
	$(RM) *.out
	$(RM) *.sum	
	rm gui-QT/*.o -rf
	rm calculator -rf


clean-all: clean
	$(RM) $(PROGS)

pack:
	tar cvf xjanot02.zip *.cpp *.h Makefile Doxyfile

unpack:
	tar -xf xjanot02.zip
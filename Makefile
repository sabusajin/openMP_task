CFLAGS=-O3 -std=c11 -g -fopenmp
CXXFLAGS=-O3 -std=c++11 -g -fopenmp
LDFLAGS=-fopenmp
ARCHIVES=libintegrate.a libfunctions.a libgen.a
LD=g++

all: reduce mergesort bubblesort lcs approx

reduce: reduce.o
	$(LD) $(LDFLAGS) reduce.o $(ARCHIVES) -o reduce

bubblesort: bubblesort.o
	$(LD) $(LDFLAGS) bubblesort.o $(ARCHIVES) -o bubblesort
	
lcs: lcs.o
	$(LD) $(LDFLAGS) lcs.o $(ARCHIVES) -o lcs

mergesort: mergesort.o
	$(LD) $(LDFLAGS) mergesort.o $(ARCHIVES) -o mergesort


libfunctions.a: functions.o
	ar rcs libfunctions.a functions.o

libintegrate.a: sequential_lib.o
		ar rcs libintegrate.a sequential_lib.o

libgen.a: gen_lib.o
	ar rcs libgen.a gen_lib.o

test_reduce: reduce
	./test_reduce.sh

assignment-openmp-loop.tgz: approx.cpp \
          assignment-openmp-loop.pdf \
          bench_reduce.sh bench_mergesort.sh bench_prefixsum.sh bench_numint.sh \
          test_reduce.sh \
	  Makefile libgen.a libfunctions.a libintegrate.a \
          reduce.cpp prefixsum.cpp mergesort.cpp numint.cpp \
          numint_seq.cpp prefixsum_seq.cpp
	tar zcvf assignment-openmp-loop.tgz \
          approx.cpp \
          assignment-openmp-loop.pdf \
          bench_reduce.sh bench_mergesort.sh bench_prefixsum.sh bench_numint.sh \
          test_reduce.sh \
	  Makefile libgen.a libfunctions.a libintegrate.a \
          reduce.cpp prefixsum.cpp mergesort.cpp numint.cpp \
          numint_seq.cpp prefixsum_seq.cpp


clean:
	-rm *.o

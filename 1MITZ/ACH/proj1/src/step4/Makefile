# Architektura procesoru (ACH 2016)
# Projekt c. 1 (nbody)
# Login: xlogin00

CC=icpc
CFLAGS=-std=c++11 -lpapi -ansi-alias 
OPT=-O2 -Wall -xavx -qopenmp-simd
#REPORT=-qopt-report=5

N=10000
DT=0.001f
STEPS=1000

PARAMS=-DN=$(N) -DDT=$(DT) -DSTEPS=$(STEPS)

INPUT=../input.dat
OUTPUT=../step0.dat

PAPI_EVENTS=PAPI_FP_OPS|PAPI_SP_OPS
PAPI_EVENTS2=PAPI_L1_TCM|PAPI_L2_TCM|PAPI_L2_TCA|PAPI_L3_TCA|PAPI_L3_TCM

all:
	$(CC) $(CFLAGS) $(OPT) $(REPORT) $(PARAMS) -c velocity.cpp
	$(CC) $(CFLAGS) $(OPT) $(REPORT) $(PARAMS) -c nbody.cpp
	$(CC) $(CFLAGS) $(OPT) $(PARAMS) -S -fsource-asm -c nbody.cpp
	$(CC) $(CFLAGS) $(OPT) $(PARAMS) velocity.o nbody.o main.cpp -o nbody
	$(CC) $(CFLAGS) gen.cpp -o gen

clean:
	rm -f *.o nbody

run:
	PAPI_EVENTS='$(PAPI_EVENTS)' ./nbody $(INPUT) $(OUTPUT)

papi:
	PAPI_EVENTS='$(PAPI_EVENTS2)' ./nbody $(INPUT) $(OUTPUT)

# Authors: Nathan Hoffman (nh662219@ohio.edu)
#          TBD     		  (xx######@ohio.edu)
# 
# Description: Makefile for cs4100 Project2, TreeBuilder Compiler.
# Date: 4/16/2024

# Compile using make -> make run, or make all.

# The .PHONY target mark the list of targets
# as none-file.
.PHONY: build run clean all


build: cmos
# @echo "build complete."

cmos: cmos.o greedy.o
# @echo "Building cmos..."
	$(CXX) -o $@ $^

cmos.o: cmos.cpp greedy.h
	$(CXX) -c -o $@ $<

greedy.o: greedy.cc greedy.h
	$(CXX) -c -o $@ $<

run: cmos
# @echo "running..."
	./cmos
# @echo "run complete."

clean:
#	@echo "Cleaning up compliation files..."
	-rm -f cmos
	-rm -f *.o
# @echo "clean complete."

all: build run
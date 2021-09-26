#!/bin/bash

g++ -std=c++17 -O3 -march=native search.cpp -o search -lntl
g++ -std=c++17 -O3 -march=native search.cpp -DMULT -o msearch -lntl
g++ -std=c++17 -O3 -march=native spect.cpp -o spect -lntl
g++ -std=c++17 -O3 -march=native spect.cpp -DMULT -o mspect -lntl
g++ -std=c++17 -O3 -march=native printdat.cpp -o printdat -lntl
g++ -std=c++17 -O3 -march=native printdat.cpp -DMULT -o mprintdat -lntl

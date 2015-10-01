#!/bin/bash

args="-Wall -std=c++11 -O3"

g++ -o bin/gcc/o/node_f.o     -c $args src/node_f.cpp
g++ -o bin/gcc/o/node_fTest.o -c $args src/node_fTest.cpp
g++ -o bin/gcc/node_fTest bin/gcc/o/node_f.o bin/gcc/o/node_fTest.o

g++ -o bin/gcc/o/node_s.o     -c $args src/node_s.cpp
g++ -o bin/gcc/o/node_sTest.o -c $args src/node_sTest.cpp
g++ -o bin/gcc/node_sTest bin/gcc/o/node_s.o bin/gcc/o/node_sTest.o

g++ -o bin/gcc/o/perfTest.o   -c $args src/perfTest.cpp
g++ -o bin/gcc/perfTest bin/gcc/o/node_f.o bin/gcc/o/node_s.o bin/gcc/o/perfTest.o


clang="clang++-3.6"

$clang -o bin/clang/o/node_f.o     -c $args src/node_f.cpp
$clang -o bin/clang/o/node_fTest.o -c $args src/node_fTest.cpp
$clang -o bin/clang/node_fTest bin/clang/o/node_f.o bin/clang/o/node_fTest.o

$clang -o bin/clang/o/node_s.o     -c $args src/node_s.cpp
$clang -o bin/clang/o/node_sTest.o -c $args src/node_sTest.cpp
$clang -o bin/clang/node_sTest bin/clang/o/node_s.o bin/clang/o/node_sTest.o

$clang -o bin/clang/o/perfTest.o   -c $args src/perfTest.cpp
$clang -o bin/clang/perfTest bin/clang/o/node_f.o bin/clang/o/node_s.o bin/clang/o/perfTest.o







cc=g++
cflags= -std=c++11 -Wall -O3

main: main.cpp
	$(cc) -o $@ $< $(cflags)

clean:
	rm main


g++ -o bin/FnPtrBlockNode/node.o -c -Wall -std=c++11 -O3 src/FnPtrBlockNode/node.cpp
g++ -o bin/FnPtrSingleNode/node.o -c -Wall -std=c++11 -O3 src/FnPtrSingleNode/node.cpp
g++ -o bin/SwitchBlockNode/node.o -c -Wall -std=c++11 -O3 src/SwitchBlockNode/node.cpp
g++ -o bin/SwitchSingleB/node.o -c -Wall -std=c++11 -O3 src/SwitchSingleB/node.cpp
g++ -o bin/SwitchSingleNode/node.o -c -Wall -std=c++11 -O3 src/SwitchSingleNode/node.cpp
g++ -o bin/llvm/test.o -c -Wall -std=c++11 -O3 src/llvm/test.cpp
g++ -o bin/perfTest.o -c -Wall -std=c++11 -O3 src/perfTest.cpp
g++ -o bin/perfTest bin/FnPtrBlockNode/node.o bin/SwitchBlockNode/node.o bin/FnPtrSingleNode/node.o bin/SwitchSingleNode/node.o bin/SwitchSingleB/node.o bin/llvm/test.o bin/perfTest.o `llvm-config --cxxflags --ldflags --libs --system-libs` -lffi

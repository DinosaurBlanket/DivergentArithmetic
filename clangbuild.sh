
clang++ -o bin/FnPtrBlockNode/node.o -c -Wall -std=c++11 -O3 src/FnPtrBlockNode/node.cpp
clang++ -o bin/FnPtrSingleNode/node.o -c -Wall -std=c++11 -O3 src/FnPtrSingleNode/node.cpp
clang++ -o bin/SwitchBlockNode/node.o -c -Wall -std=c++11 -O3 src/SwitchBlockNode/node.cpp
clang++ -o bin/SwitchSingleB/node.o -c -Wall -std=c++11 -O3 src/SwitchSingleB/node.cpp
clang++ -o bin/SwitchSingleNode/node.o -c -Wall -std=c++11 -O3 src/SwitchSingleNode/node.cpp
clang++ -o bin/perfTest.o -c -Wall -std=c++11 -O3 src/perfTest.cpp
clang++ -o bin/perfTest_clang bin/FnPtrBlockNode/node.o bin/SwitchBlockNode/node.o bin/FnPtrSingleNode/node.o bin/SwitchSingleNode/node.o bin/SwitchSingleB/node.o bin/perfTest.o

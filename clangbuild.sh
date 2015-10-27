
clang++-3.6 -o bin/FnPtrBlockNode/node.o -c -Wall -std=c++11 -O3 src/FnPtrBlockNode/node.cpp
clang++-3.6 -o bin/FnPtrSingleNode/node.o -c -Wall -std=c++11 -O3 src/FnPtrSingleNode/node.cpp
clang++-3.6 -o bin/SwitchBlockNode/node.o -c -Wall -std=c++11 -O3 src/SwitchBlockNode/node.cpp
clang++-3.6 -o bin/SwitchSingleNode/node.o -c -Wall -std=c++11 -O3 src/SwitchSingleNode/node.cpp
clang++-3.6 -o bin/perfTest.o -c -Wall -std=c++11 -O3 src/perfTest.cpp
clang++-3.6 -o bin/perfTest bin/FnPtrBlockNode/node.o bin/SwitchBlockNode/node.o bin/FnPtrSingleNode/node.o bin/SwitchSingleNode/node.o bin/perfTest.o

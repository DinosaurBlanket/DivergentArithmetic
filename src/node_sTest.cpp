


#include <iostream>
using std::cout;
using std::endl;
#include <iomanip>
#include <vector>
using std::vector;
#include "node_s.hpp"

int main(int argc, char const **argv) {
  
  node_s litA = node_s(2);cout << "litA : " << &litA << endl;
  node_s litB = node_s(1);cout << "litB : " << &litB << endl;
  vector<node_s> addArgs = {litA, litB};
  node_s adder = node_s(ni_add, addArgs);cout << "adder: " << &adder << endl;
  node_s globalIndexNode = node_s(ni_globalIndex);cout << "GIN  : " << &adder << endl;
  vector<node_s> mulArgs = {adder, globalIndexNode};
  node_s root = node_s(ni_mul, mulArgs);cout << "root : " << &root << endl;
  
  
  //vector<node_s> addArgs = {node_s(1), node_s(2)};
  //node_s adder = node_s(ni_add, addArgs);cout << "adder: " << &adder << endl;
  //node_s globalIndexNode = node_s(ni_globalIndex);cout << "GIN  : " << &adder << endl;
  //vector<node_s> mulArgs = {node_s(ni_add, &addArgs[0], 2), node_s(ni_globalIndex)};
  //node_s root = node_s(ni_mul, mulArgs);cout << "root : " << &root << endl;
  
  
  //node_s addArgs[] = {node_s(1), node_s(2)};
  //node_s adder = node_s(ni_add, &addArgs[0], 2);cout << "adder: " << &adder << endl;
  //node_s globalIndexNode = node_s(ni_globalIndex);cout << "GIN  : " << &adder << endl;
  //node_s mulArgs[] = {node_s(ni_add, &addArgs[0], 2), node_s(ni_globalIndex)};
  //node_s root = node_s(ni_mul, &mulArgs[0], 2);cout << "root : " << &root << endl;
  
  
  
  vector<num> data;
  data.resize(1000, 0);
  root.outputTo(data);
  
  const uint pitch = 10;
  for (uint i = 0; i < data.size(); i++) {
    cout << std::setw(7) << std::setfill(' ') << data[i];
    if ((i+1)%pitch == 0) cout << endl;
  }
  return 0;
}

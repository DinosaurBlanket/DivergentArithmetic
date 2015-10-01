

#include <iostream>
using std::cout;
using std::endl;
#include <iomanip>
#include <vector>
using std::vector;
#include "node.hpp"

int main(int argc, char const **argv) {
  
  // this works now
  node litA = node(2);cout << "litA : " << &litA << endl;
  node litB = node(1);cout << "litB : " << &litB << endl;
  vector<node> addArgs = {litA, litB};
  node adder = node(nf_add, addArgs);cout << "adder: " << &adder << endl;
  node globalIndexNode = node(nf_globalIndex);cout << "GIN  : " << &adder << endl;
  vector<node> mulArgs = {adder, globalIndexNode};
  node root = node(nf_mul, mulArgs);cout << "root : " << &root << endl;
  
  
  // this works
  //vector<node> addArgs = {node(1), node(2)};
  //node adder = node(nf_add, addArgs);cout << "adder: " << &adder << endl;
  //node globalIndexNode = node(nf_globalIndex);cout << "GIN  : " << &adder << endl;
  //vector<node> mulArgs = {node(nf_add, &addArgs[0], 2), node(nf_globalIndex)};
  //node root = node(nf_mul, mulArgs);cout << "root : " << &root << endl;
  
  
  // this also works
  //node addArgs[] = {node(1), node(2)};
  //node adder = node(nf_add, &addArgs[0], 2);cout << "adder: " << &adder << endl;
  //node globalIndexNode = node(nf_globalIndex);cout << "GIN  : " << &adder << endl;
  //node mulArgs[] = {node(nf_add, &addArgs[0], 2), node(nf_globalIndex)};
  //node root = node(nf_mul, &mulArgs[0], 2);cout << "root : " << &root << endl;
  
  
  
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

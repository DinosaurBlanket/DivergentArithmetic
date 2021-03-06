// g++ playingWithFnPtrs.cpp -o playingWithFnPtrs -std=c++11 -Wall -O3

#include <iostream>
using std::cout;
using std::endl;
#include <iomanip>
using std::setfill;
using std::setw;
#include <chrono>
#include "FnPtrBlockNode/node.hpp"
#include "SwitchBlockNode/node.hpp"
#include "FnPtrSingleNode/node.hpp"
#include "SwitchSingleNode/node.hpp"
#include "SwitchSingleB/node.hpp"
#include "lightning/test.h"
//#include "llvm/test.hpp"


num _add(num a, num b) {return a + b;}
num _sub(num a, num b) {return a - b;}
num _mul(num a, num b) {return a * b;}
num _div(num a, num b) {return a / b;}


enum switch_op {so_add, so_sub, so_mul, so_div};
num everyFn(switch_op op, num a, num b) {
  switch (op) {
    case so_add: return a+b;
    case so_sub: return a-b;
    case so_mul: return a*b;
    case so_div: return a/b;
  }
  return 0;
}


#define _checkData \
for (uint i = 0; i < dataSize; i++) {\
  if (data[i] != i) {\
    cout << "incorrect data at " << i << ": " << data[i] << endl;\
    exit(1);\
  }\
  data[i] = -1;\
}

#define intPrintWidth 8

int main(int argc, char const **argv) {
  long dataSize = 2048000;
  num data[dataSize];
  cout << "dataSize: " << dataSize << endl;
  for (uint i = 0; i < dataSize; i++) {data[i] = 0;}

  //control
  {
  	auto start_time = std::chrono::high_resolution_clock::now();
    for (uint i = 0; i < dataSize; i++) {
      data[i] = _add(i, _sub(_mul(2, _div(i, 2)), i));
    }
  	auto end_time = std::chrono::high_resolution_clock::now();
    cout << "call direct     : " << setfill(' ') << setw(intPrintWidth) <<
    std::chrono::duration_cast<std::chrono::microseconds>(
      end_time - start_time
    ).count() << " microseconds" << endl;
  }

  //every fn ptr per iteration
  {
    //the conditions are to prevent the compiler from inlining
    const op opAdd = argc > 1000 ? _sub : _add;
    const op opSub = argc > 1000 ? _mul : _sub;
    const op opMul = argc > 1000 ? _div : _mul;
    const op opDiv = argc > 1000 ? _add : _div;
  	auto start_time = std::chrono::high_resolution_clock::now();
    for (uint i = 0; i < dataSize; i++) {
      data[i] = opAdd(i, opSub(opMul(2, opDiv(i, 2)), i));
    }
  	auto end_time = std::chrono::high_resolution_clock::now();
    cout << "fn ptr direct   : " << setfill(' ') << setw(intPrintWidth) <<
    std::chrono::duration_cast<std::chrono::microseconds>(
      end_time - start_time
    ).count() << " microseconds" << endl;
  }
  _checkData

  //switch per iteration
  {
    const switch_op a = argc > 1000 ? so_sub : so_add;
    const switch_op s = argc > 1000 ? so_mul : so_sub;
    const switch_op m = argc > 1000 ? so_div : so_mul;
    const switch_op d = argc > 1000 ? so_add : so_div;
  	auto start_time = std::chrono::high_resolution_clock::now();
    for (uint i = 0; i < dataSize; i++) {
      data[i] = everyFn(a, i, everyFn(s, everyFn(m, 2, everyFn(d, i, 2)), i));
    }
  	auto end_time = std::chrono::high_resolution_clock::now();
    cout << "switch direct   : " << setfill(' ') << setw(intPrintWidth) <<
    std::chrono::duration_cast<std::chrono::microseconds>(
      end_time - start_time
    ).count() << " microseconds" << endl;
  }
  _checkData


  //FnPtrBlockNode
  {
    std::vector<node_fb> divArgs_fb = {node_fb(nf_globalIndex), node_fb( 2)};
    node_fb divn_fb = node_fb(nf_div, divArgs_fb);
    std::vector<node_fb> mulArgs_fb = {node_fb( 2), divn_fb};
    node_fb muln_fb = node_fb(nf_mul, mulArgs_fb);
    std::vector<node_fb> subArgs_fb = {muln_fb, node_fb(nf_globalIndex)};
    node_fb subn_fb = node_fb(nf_sub, subArgs_fb);
    std::vector<node_fb> addArgs_fb = {node_fb(nf_globalIndex), subn_fb};
    node_fb addn_fb = node_fb(nf_add, addArgs_fb);
    auto start_time = std::chrono::high_resolution_clock::now();
    addn_fb.outputTo(data, dataSize);
  	auto end_time = std::chrono::high_resolution_clock::now();
    cout << "FnPtrBlockNode  : " << setfill(' ') << setw(intPrintWidth) <<
    std::chrono::duration_cast<std::chrono::microseconds>(
      end_time - start_time
    ).count() << " microseconds" << endl;
  }
  _checkData

  //SwitchBlockNode
  {
    std::vector<node_sb> divArgs = {node_sb(ni_globalIndex), node_sb( 2)};
    node_sb divn = node_sb(ni_div, divArgs);
    std::vector<node_sb> mulArgs = {node_sb( 2), divn};
    node_sb muln = node_sb(ni_mul, mulArgs);
    std::vector<node_sb> subArgs = {muln, node_sb(ni_globalIndex)};
    node_sb subn = node_sb(ni_sub, subArgs);
    std::vector<node_sb> addArgs = {node_sb(ni_globalIndex), subn};
    node_sb addn = node_sb(ni_add, addArgs);
    auto start_time = std::chrono::high_resolution_clock::now();
    addn.outputTo(data, dataSize);
  	auto end_time = std::chrono::high_resolution_clock::now();
    cout << "SwitchBlockNode : " << setfill(' ') << setw(intPrintWidth) <<
    std::chrono::duration_cast<std::chrono::microseconds>(
      end_time - start_time
    ).count() << " microseconds" << endl;
  }
  _checkData

  //FnPtrSingleNode
  {
    std::vector<node_fs> divArgs_fs = {node_fs(fs_globalIndex), node_fs( 2)};
    node_fs divn_fs = node_fs(fs_div, divArgs_fs);
    std::vector<node_fs> mulArgs_fs = {node_fs( 2), divn_fs};
    node_fs muln_fs = node_fs(fs_mul, mulArgs_fs);
    std::vector<node_fs> subArgs_fs = {muln_fs, node_fs(fs_globalIndex)};
    node_fs subn_fs = node_fs(fs_sub, subArgs_fs);
    std::vector<node_fs> addArgs_fs = {node_fs(fs_globalIndex), subn_fs};
    node_fs addn_fs = node_fs(fs_add, addArgs_fs);
    auto start_time = std::chrono::high_resolution_clock::now();
    addn_fs.outputTo(data, dataSize);
  	auto end_time = std::chrono::high_resolution_clock::now();
    cout << "FnPtrSingleNode : " << setfill(' ') << setw(intPrintWidth) <<
    std::chrono::duration_cast<std::chrono::microseconds>(
      end_time - start_time
    ).count() << " microseconds" << endl;
  }
  _checkData

  //SwitchSingleNode
  {
    const uint nodeCount = 9;
    ssn_node nodes[nodeCount];
    nodes[0].ret  = 2;
    nodes[0].iden = I_lit;
    nodes[1].iden = I_index;
    nodes[2].iden = I_div;
    nodes[2].args[0] = 1;
    nodes[2].args[1] = 0;
    nodes[3].ret  = 2;
    nodes[3].iden = I_lit;
    nodes[4].iden = I_index;
    nodes[5].iden = I_mul;
    nodes[5].args[0] = 3;
    nodes[5].args[1] = 2;
    nodes[6].iden = I_sub;
    nodes[6].args[0] = 5;
    nodes[6].args[1] = 4;
    nodes[7].iden = I_index;
    nodes[8].iden = I_add;
    nodes[8].args[0] = 7;
    nodes[8].args[1] = 6;

    auto start_time = std::chrono::high_resolution_clock::now();
    ssn_outputTo(data, dataSize, nodes, nodeCount);
  	auto end_time = std::chrono::high_resolution_clock::now();
    cout << "SwitchSingleNode: " << setfill(' ') << setw(intPrintWidth) <<
    std::chrono::duration_cast<std::chrono::microseconds>(
      end_time - start_time
    ).count() << " microseconds" << endl;
  }
  _checkData


  //SwitchSingleNodeB
  {
    /*
    add         8
      i         7
      sub       6
        mul     5
          2     4
          div   3
            i   2
            2   1
        i       0
    */
    const uint nodeCount = 9;
    Iden nodes[nodeCount];
    const uint litCount = 2;
    num lits[litCount];
    nodes[0] = I_lit;   lits [0] = 2;
    nodes[1] = I_index;
    nodes[2] = I_div;
    nodes[3] = I_lit;   lits [1] = 2;
    nodes[4] = I_index;
    nodes[5] = I_mul;
    nodes[6] = I_sub;
    nodes[7] = I_index;
    nodes[8] = I_add;

    auto start_time = std::chrono::high_resolution_clock::now();
    ssnb_outputTo(data, dataSize, nodes, nodeCount, lits, litCount);
  	auto end_time = std::chrono::high_resolution_clock::now();
    cout << "SwitchSingleB   : " << setfill(' ') << setw(intPrintWidth) <<
    std::chrono::duration_cast<std::chrono::microseconds>(
      end_time - start_time
    ).count() << " microseconds" << endl;
  }
  _checkData

  
  //lightning
  lightningTest(data, dataSize);
  _checkData
  
  
  /*
  //llvm
  {
    auto start_time = std::chrono::high_resolution_clock::now();
    initLLVM();
  	auto end_time = std::chrono::high_resolution_clock::now();
    auto compileTime = end_time - start_time;

    start_time = std::chrono::high_resolution_clock::now();
    runLLVM(data, dataSize);
  	end_time = std::chrono::high_resolution_clock::now();

    cout << "LLVM test       : " << setfill(' ') << setw(intPrintWidth) <<
    std::chrono::duration_cast<std::chrono::microseconds>(
      end_time - start_time
    ).count() << " microseconds, compiled in: " <<
    std::chrono::duration_cast<std::chrono::microseconds>(
      compileTime
    ).count() << " microseconds" << endl;

    cleanupLLVM();
  }
  _checkData
  */

  return 0;
}

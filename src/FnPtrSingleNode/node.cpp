
#include "node.hpp"
#include "../options.h"
#include <cstring>

#if LOG_NODE_CONSTR_DESTR
#include <iostream>
using std::cout;
using std::endl;
#endif


void node_fs::outputTo(num *destData, uint destDataSize) {
  node_fs dummyNode = node_fs(fs_dummy, this, 1);
  #if LOG_NODE_CONSTR_DESTR
  cout << "dummy: " << &dummyNode << endl;
  #endif
  for (uint i = 0; i < destDataSize; i++) {
    dummyNode.output(nullptr, 0, i);
    destData[i] = dummyNode.inputData[0];
  }
}
void node_fs::outputTo(std::vector<num> &destData) {
  outputTo(destData.data(), destData.size());
}

node_fs::node_fs(num literal) :
  args(nullptr),
  argCount(0),
  nf(fs_literal)
{
  inputData[0] = literal;
  for (uint i = 1; i < maxArgs; i++) inputData[i] = 0;
}

node_fs::node_fs(nodeFunc_fs nfIn) :
  args(nullptr),
  argCount(0),
  nf(nfIn)
{}

node_fs::node_fs(nodeFunc_fs nfIn, node_fs *argsIn, uint argCountIn) :
  args(argsIn),
  argCount(argCountIn),
  nf(nfIn)
{}

node_fs::node_fs(nodeFunc_fs nfIn, std::vector<node_fs> &argsIn) :
  args(argsIn.data()),
  argCount(argsIn.size()),
  nf(nfIn)
{}

node_fs::node_fs(const node_fs &n) :
  args(n.args),
  argCount(n.argCount),
  nf(n.nf)
{
  #if LOG_NODE_CONSTR_DESTR
  cout << "copying from " << &n << " to " << this << endl;
  #endif
  for (uint i = 0; i < maxArgs; i++) inputData[i] = n.inputData[i];
}

node_fs::~node_fs() {}

void node_fs::output(node_fs *dest, uint destOffset, uint globalIndexOffset) {
  for (uint i = 0; i < argCount; i++) {
    args[i].output(this, i, globalIndexOffset);
  }
  nf(dest, destOffset, this, globalIndexOffset);
}



void fs_literal(_nodeFuncParams_fs) {
  dest->inputData[destOffset] = src->inputData[0];
}

void fs_globalIndex(_nodeFuncParams_fs) {
  dest->inputData[destOffset] = globalIndexOffset;
}


#define _binaryOp(_name, _op) \
void _name(_nodeFuncParams_fs) {\
  dest->inputData[destOffset] = src->inputData[0] _op src->inputData[1];\
}

_binaryOp(fs_add, +)
_binaryOp(fs_sub, -)
_binaryOp(fs_mul, *)
_binaryOp(fs_div, /)


void fs_dummy(_nodeFuncParams_fs) {}


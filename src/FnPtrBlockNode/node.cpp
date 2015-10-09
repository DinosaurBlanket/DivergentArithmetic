
#include "node.hpp"
#include "../options.h"
#include <cstring>

#if LOG_NODE_CONSTR_DESTR
#include <iostream>
using std::cout;
using std::endl;
#endif


void node_fb::outputTo(num *destData, uint destDataSize) {
  node_fb dummyNode = node_fb(nf_dummy, this, 1);
  #if LOG_NODE_CONSTR_DESTR
  cout << "dummy: " << &dummyNode << endl;
  #endif
  num *dummyNodesData = dummyNode.inputData;
  uint i = 0;
  for (; i+chunkSize <= destDataSize; i += chunkSize) {
    dummyNode.inputData = &destData[i];
    output(&dummyNode, 0, i);
  }
  dummyNode.inputData = dummyNodesData;
  uint remainder = destDataSize % chunkSize;
  if (remainder) {
    output(&dummyNode, 0, i);
    std::memcpy(&destData[i], dummyNode.inputData, remainder*sizeof(num));
  }
}
void node_fb::outputTo(std::vector<num> &destData) {
  outputTo(destData.data(), destData.size());
}

node_fb::node_fb(num literal) :
  singleData(literal),
  inputData(nullptr),
  inputDataCount(0),
  args(nullptr),
  argCount(0),
  nf(nf_literal)
{}

node_fb::node_fb(nodeFunc nfIn) :
  singleData(0),
  inputData(nullptr),
  inputDataCount(0),
  args(nullptr),
  argCount(0),
  nf(nfIn)
{}

node_fb::node_fb(nodeFunc nfIn, node_fb *argsIn, uint argCountIn) :
  singleData(0),
  inputDataCount(argCountIn*chunkSize),
  args(argsIn),
  argCount(argCountIn),
  nf(nfIn)
{
  inputData = new num[inputDataCount];
}

node_fb::node_fb(nodeFunc nfIn, std::vector<node_fb> &argsIn) :
  singleData(0),
  inputDataCount(argsIn.size()*chunkSize),
  args(argsIn.data()),
  argCount(argsIn.size()),
  nf(nfIn)
{
  inputData = new num[inputDataCount];
}

node_fb::node_fb(const node_fb &n) :
  singleData(n.singleData),
  inputDataCount(n.inputDataCount),
  args(n.args),
  argCount(n.argCount),
  nf(n.nf)
{
  #if LOG_NODE_CONSTR_DESTR
  cout << "copying from " << &n << " to " << this << endl;
  #endif
  if (inputDataCount) inputData = new num[inputDataCount];
  else inputData = nullptr;
}

node_fb::~node_fb() {
  #if LOG_NODE_CONSTR_DESTR
  cout << "deleting inputData from: " << this << endl;
  #endif
  if (inputData) delete[] inputData;
}

void node_fb::output(node_fb *dest, uint destOffset, uint globalIndexOffset) {
  for (uint i = 0; i < argCount; i++) {
    args[i].output(this, i, globalIndexOffset);
  }
  nf(dest, destOffset, this, globalIndexOffset);
}



void nf_literal(_nodeFuncParams) {
  const num out = src->singleData;
  #if INTERLACE_INPUT_DATA
    for (
      uint di = destOffset;
      di < dest->inputDataCount;
      di += dest->argCount
    ) {
      dest->inputData[di] = out;
    }
  #else
    uint di = destOffset * chunkSize;
    const uint diEnd = di + chunkSize;
    for (; di < diEnd; di++) dest->inputData[di] = out;
  #endif
}

void nf_globalIndex(_nodeFuncParams) {
  #if INTERLACE_INPUT_DATA
    uint di = destOffset, gi = globalIndexOffset;
    for (; di < dest->inputDataCount; di += dest->argCount, gi++) {
      dest->inputData[di] = gi;
    }
  #else
    uint di = destOffset * chunkSize;
    const uint diEnd = di + chunkSize;
    uint gi = globalIndexOffset;
    for (; di < diEnd; di++, gi++) dest->inputData[di] = gi;
  #endif
}


#if INTERLACE_INPUT_DATA

#define _binaryOp(_name, _op) \
void _name(_nodeFuncParams) {\
  uint di = destOffset, si = 0;\
  for (;\
    di < dest->inputDataCount;\
    di += dest->argCount, si += src->argCount\
  ) {\
    dest->inputData[di] = src->inputData[si] _op src->inputData[si+1];\
  }\
}

#else

#define _binaryOp(_name, _op) \
void _name(_nodeFuncParams) {\
  uint di = destOffset * chunkSize;\
  const uint diEnd = di + chunkSize;\
  uint siA = 0;\
  uint siB = chunkSize;\
  for (; di < diEnd; di++, siA++, siB++) {\
    dest->inputData[di] = src->inputData[siA] _op src->inputData[siB];\
  }\
}

#endif

_binaryOp(nf_add, +)
_binaryOp(nf_sub, -)
_binaryOp(nf_mul, *)
_binaryOp(nf_div, /)


void nf_dummy(_nodeFuncParams) {}


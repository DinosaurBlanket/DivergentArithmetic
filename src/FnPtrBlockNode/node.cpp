

#include "node.hpp"
#include <cstring>

#if LOG_NODE_CONSTR_DESTR
#include <iostream>
using std::cout;
using std::endl;
#endif

const uint chunkSize = 256;

void node_fb::outputTo(num *destData, uint destDataSize) {
  node_fb dummyNode = node_fb(nf_dummy, this, 1);
  #if LOG_NODE_CONSTR_DESTR
  cout << "dummy: " << &dummyNode << endl;
  #endif
  num *dummyNodesData = dummyNode.inputData;
  uint i = 0;
  for (; i+chunkSize <= destDataSize; i += chunkSize) {
    dummyNode.inputData = &destData[i];
    this->output(&dummyNode, 0, i);
  }
  dummyNode.inputData = dummyNodesData;
  uint remainder = destDataSize % chunkSize;
  if (remainder) {
    this->output(&dummyNode, 0, i);
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
  for (uint di = destOffset; di < dest->inputDataCount; di += dest->argCount) {
    dest->inputData[di] = src->singleData;
  }
}


void nf_add(_nodeFuncParams) {
  uint di = destOffset, si = 0;
  for (; di < dest->inputDataCount; di += dest->argCount, si += src->argCount) {
    dest->inputData[di] = src->inputData[si] + src->inputData[si+1];
  }
}
void nf_sub(_nodeFuncParams) {
  uint di = destOffset, si = 0;
  for (; di < dest->inputDataCount; di += dest->argCount, si += src->argCount) {
    dest->inputData[di] = src->inputData[si] - src->inputData[si+1];
  }
}
void nf_mul(_nodeFuncParams) {
  uint di = destOffset, si = 0;
  for (; di < dest->inputDataCount; di += dest->argCount, si += src->argCount) {
    dest->inputData[di] = src->inputData[si] * src->inputData[si+1];
  }
}
void nf_div(_nodeFuncParams) {
  uint di = destOffset, si = 0;
  for (; di < dest->inputDataCount; di += dest->argCount, si += src->argCount) {
    dest->inputData[di] = src->inputData[si] / src->inputData[si+1];
  }
}

void nf_globalIndex(_nodeFuncParams) {
  uint di = destOffset, gi = globalIndexOffset;
  for (; di < dest->inputDataCount; di += dest->argCount, gi++) {
    dest->inputData[di] = gi;
  }
}

void nf_dummy(_nodeFuncParams) {}


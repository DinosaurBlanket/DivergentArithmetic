

#include <iostream>
using std::cout;
using std::endl;
#include "node.hpp"
#include <cstring>

const uint chunkSize = 256;

void node::outputTo(num *destData, uint destDataSize) {
  node dummyNode = node(nf_dummy, this, 1);cout << "dummy: " << &dummyNode << endl;
  num *dummyNodesData = dummyNode.inputData;
  uint i = 0;
  for (; i+chunkSize < destDataSize; i += chunkSize) {
    dummyNode.inputData = &destData[i];
    dummyNode.output(nullptr, 0, i);
  }
  dummyNode.inputData = dummyNodesData;
  uint remainder = destDataSize % chunkSize;
  if (remainder) {
    dummyNode.output(nullptr, 0, i);
    std::memcpy(&destData[i], dummyNode.inputData, remainder*sizeof(num));
  }
}
void node::outputTo(std::vector<num> &destData) {
  outputTo(destData.data(), destData.size());
}

node::node(num literal) :
  singleData(literal),
  inputData(nullptr),
  inputDataCount(0),
  args(nullptr),
  argCount(0),
  nf(nf_literal)
{}

node::node(nodeFunc nfIn) :
  singleData(0),
  inputData(nullptr),
  inputDataCount(0),
  args(nullptr),
  argCount(0),
  nf(nfIn)
{}

node::node(nodeFunc nfIn, node *argsIn, uint argCountIn) :
  singleData(0),
  inputDataCount(argCountIn*chunkSize),
  args(argsIn),
  argCount(argCountIn),
  nf(nfIn)
{
  inputData = new num[inputDataCount];
}

node::node(nodeFunc nfIn, std::vector<node> &argsIn) :
  singleData(0),
  inputDataCount(argsIn.size()*chunkSize),
  args(argsIn.data()),
  argCount(argsIn.size()),
  nf(nfIn)
{
  inputData = new num[inputDataCount];
}

node::node(const node &n) :
  singleData(n.singleData),
  inputDataCount(n.inputDataCount),
  args(n.args),
  argCount(n.argCount),
  nf(n.nf)
{
  cout << "copying from " << &n << " to " << this << endl;
  if (inputDataCount) inputData = new num[inputDataCount];
}

node::~node() {
  cout << "deleting inputData from: " << this << endl;
  if (inputData) delete[] inputData;
}

void node::output(node *dest, uint destOffset, uint globalIndexOffset) {
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
void nf_mul(_nodeFuncParams) {
  uint di = destOffset, si = 0;
  for (; di < dest->inputDataCount; di += dest->argCount, si += src->argCount) {
    dest->inputData[di] = src->inputData[si] * src->inputData[si+1];
  }
}

void nf_globalIndex(_nodeFuncParams) {
  uint di = destOffset, gi = globalIndexOffset;
  for (; di < dest->inputDataCount; di += dest->argCount, gi++) {
    dest->inputData[di] = gi;
  }
}

void nf_dummy(_nodeFuncParams) {}


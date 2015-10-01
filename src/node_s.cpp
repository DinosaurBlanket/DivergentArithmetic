

#include "node_s.hpp"
#include <cstring>
#include <iostream>
using std::cout;
using std::endl;


const uint chunkSize = 256;

void node_s::outputTo(num *destData, uint destDataSize) {
  node_s dummyNode = node_s(ni_dummy, this, 1);
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
void node_s::outputTo(std::vector<num> &destData) {
  outputTo(destData.data(), destData.size());
}

node_s::node_s(num literal) :
  singleData(literal),
  inputData(nullptr),
  inputDataCount(0),
  args(nullptr),
  argCount(0),
  ni(ni_literal)
{}

node_s::node_s(nodeIden niIn) :
  singleData(0),
  inputData(nullptr),
  inputDataCount(0),
  args(nullptr),
  argCount(0),
  ni(niIn)
{}

node_s::node_s(nodeIden niIn, node_s *argsIn, uint argCountIn) :
  singleData(0),
  inputDataCount(argCountIn*chunkSize),
  args(argsIn),
  argCount(argCountIn),
  ni(niIn)
{
  inputData = new num[inputDataCount];
}

node_s::node_s(nodeIden niIn, std::vector<node_s> &argsIn) :
  singleData(0),
  inputDataCount(argsIn.size()*chunkSize),
  args(argsIn.data()),
  argCount(argsIn.size()),
  ni(niIn)
{
  inputData = new num[inputDataCount];
}

node_s::node_s(const node_s &n) :
  singleData(n.singleData),
  inputDataCount(n.inputDataCount),
  args(n.args),
  argCount(n.argCount),
  ni(n.ni)
{
  if (inputDataCount) inputData = new num[inputDataCount];
  else inputData = nullptr;
}

node_s::~node_s() {
  if (inputData) delete[] inputData;
}

void node_s::output(node_s *dest, uint destOffset, uint globalIndexOffset) {
  for (uint i = 0; i < argCount; i++) {
    args[i].output(this, i, globalIndexOffset);
  }
  //nf(dest, destOffset, this, globalIndexOffset);
  switch (ni) {
    case ni_literal: {
      for (
        uint di = destOffset;
        di < dest->inputDataCount;
        di += dest->argCount
      ) {
        dest->inputData[di] = singleData;
      }
      break;
    }
    case ni_add: {
      uint di = destOffset, si = 0;
      for (; di < dest->inputDataCount; di += dest->argCount, si += argCount) {
        dest->inputData[di] = inputData[si] + inputData[si+1];
      }
      break;
    }
    case ni_sub: {
      uint di = destOffset, si = 0;
      for (; di < dest->inputDataCount; di += dest->argCount, si += argCount) {
        dest->inputData[di] = inputData[si] - inputData[si+1];
      }
      break;
    }
    case ni_mul: {
      uint di = destOffset, si = 0;
      for (; di < dest->inputDataCount; di += dest->argCount, si += argCount) {
        dest->inputData[di] = inputData[si] * inputData[si+1];
      }
      break;
    }
    case ni_div: {
      uint di = destOffset, si = 0;
      for (; di < dest->inputDataCount; di += dest->argCount, si += argCount) {
        dest->inputData[di] = inputData[si] / inputData[si+1];
      }
      break;
    }
    case ni_globalIndex: {
      uint di = destOffset, gi = globalIndexOffset;
      for (; di < dest->inputDataCount; di += dest->argCount, gi++) {
        dest->inputData[di] = gi;
      }
      break;
    }
    case ni_dummy: break;
    default: cout << "error: unknown node identity" << endl;
  }
}




#include "node.hpp"
#include <cstring>
#include <iostream>
using std::cout;
using std::endl;


const uint chunkSize = 256;

void node_sb::outputTo(num *destData, uint destDataSize) {
  node_sb dummyNode = node_sb(ni_dummy, this, 1);
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
void node_sb::outputTo(std::vector<num> &destData) {
  outputTo(destData.data(), destData.size());
}

node_sb::node_sb(num literal) :
  singleData(literal),
  inputData(nullptr),
  inputDataCount(0),
  args(nullptr),
  argCount(0),
  ni(ni_literal)
{}

node_sb::node_sb(nodeIden niIn) :
  singleData(0),
  inputData(nullptr),
  inputDataCount(0),
  args(nullptr),
  argCount(0),
  ni(niIn)
{}

node_sb::node_sb(nodeIden niIn, node_sb *argsIn, uint argCountIn) :
  singleData(0),
  inputDataCount(argCountIn*chunkSize),
  args(argsIn),
  argCount(argCountIn),
  ni(niIn)
{
  inputData = new num[inputDataCount];
}

node_sb::node_sb(nodeIden niIn, std::vector<node_sb> &argsIn) :
  singleData(0),
  inputDataCount(argsIn.size()*chunkSize),
  args(argsIn.data()),
  argCount(argsIn.size()),
  ni(niIn)
{
  inputData = new num[inputDataCount];
}

node_sb::node_sb(const node_sb &n) :
  singleData(n.singleData),
  inputDataCount(n.inputDataCount),
  args(n.args),
  argCount(n.argCount),
  ni(n.ni)
{
  if (inputDataCount) inputData = new num[inputDataCount];
  else inputData = nullptr;
}

node_sb::~node_sb() {
  if (inputData) delete[] inputData;
}

void node_sb::output(node_sb *dest, uint destOffset, uint globalIndexOffset) {
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


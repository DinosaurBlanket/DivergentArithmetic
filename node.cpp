

#include <iostream>
using std::cout;
using std::endl;
#include "node.hpp"
#include <cstring>

const uint chunkSize = 256;

void outputTo(node &root, num *destData, uint destDataSize) {
  node dummyNode = node(nf_dummy, &root, 1);cout << "dummy: " << &dummyNode << endl;
  for (uint i = 0; i < destDataSize; i += chunkSize) {
    dummyNode.output(nullptr, 0, i);
    uint copyCount = i+chunkSize > destDataSize ? destDataSize-i : chunkSize;
    std::memcpy(&destData[i], dummyNode.inputData, copyCount*sizeof(num));
  }
}
void outputTo(node &root, std::vector<num> &destData) {
  outputTo(root, destData.data(), destData.size());
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



/*
dvrgArithNode::dvrgArithNode() {}
dvrgArithNode::dvrgArithNode(nodeFunc nfIn, vector<dvrgArithNode> &argsIn)
: nf(nfIn), args(argsIn) {
  inputData.resize(args.size()*packetSize);
}
dvrgArithNode::dvrgArithNode(nodeFunc nfIn) : nf(nfIn) {
  inputData.resize(packetSize);
}
dvrgArithNode::dvrgArithNode(num literal) {
  inputData.resize(packetSize);
  for (uint i = 0; i < packetSize; i ++) inputData[i] = literal;
}
void dvrgArithNode::output(_nodeFuncParams) {
  for (uint arg = 0; arg < args.size(); arg++) {
    args[arg].output(*this, arg, args[arg]);
  }
  nf(dest, destOffset, src);
}


#define INTERLEAVE_PACKET_DATA true

void nf_literal(_nodeFuncParams) {
  #if INTERLEAVE_PACKET_DATA
  uint di = 0, si = 0;
  for (;
    di < dest.inputData.size();
    di += dest.inputData.args.size(), si += 1
  ) {
    dest.inputData[di] = src.inputData[si]
  }
  #else
  #endif
}

void nf_add(_nodeFuncParams) {
  #if INTERLEAVE_PACKET_DATA
  uint di = 0, si = 0;
  for (;
    di < dest.inputData.size();
    di += dest.inputData.args.size(), si += src.inputData.args.size()
  ) {
    dest.inputData[di] = src.inputData[si] + src.inputData[si+1];
  }
  #else
  #endif
}
*/
#pragma once
#include <vector>
#include "num.h"

#define LOG_NODE_CONSTR_DESTR false

struct node;

#define _nodeFuncParams \
  node *dest, uint destOffset, node *src, uint globalIndexOffset
typedef void (*nodeFunc)(_nodeFuncParams);

struct node {
  num      singleData;
  num     *inputData;
  uint     inputDataCount;
  node    *args;
  uint     argCount;
  nodeFunc nf;
  node(num literal);
  node(nodeFunc nfIn);
  node(nodeFunc nfIn, node *argsIn, uint argCountIn);
  node(nodeFunc nfIn, std::vector<node> &argsIn);
  node(const node &n);
  ~node();
  void output(node *dest, uint destOffset, uint globalIndexOffset);
  void outputTo(num *destData, uint destDataSize);
  void outputTo(std::vector<num> &destData);
};


void nf_add(_nodeFuncParams);
void nf_sub(_nodeFuncParams);
void nf_mul(_nodeFuncParams);
void nf_div(_nodeFuncParams);
void nf_literal(_nodeFuncParams);
void nf_globalIndex(_nodeFuncParams);
void nf_dummy(_nodeFuncParams);



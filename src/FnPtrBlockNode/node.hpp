#pragma once
#include <vector>
#include "../num.h"

#define LOG_NODE_CONSTR_DESTR false

struct node_fb;

#define _nodeFuncParams \
  node_fb *dest, uint destOffset, node_fb *src, uint globalIndexOffset
typedef void (*nodeFunc)(_nodeFuncParams);

struct node_fb {
  num      singleData;
  num     *inputData;
  uint     inputDataCount;
  node_fb *args;
  uint     argCount;
  nodeFunc nf;
  node_fb(num literal);
  node_fb(nodeFunc nfIn);
  node_fb(nodeFunc nfIn, node_fb *argsIn, uint argCountIn);
  node_fb(nodeFunc nfIn, std::vector<node_fb> &argsIn);
  node_fb(const node_fb &n);
  ~node_fb();
  void output(node_fb *dest, uint destOffset, uint globalIndexOffset);
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



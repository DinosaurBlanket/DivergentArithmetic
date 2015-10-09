#pragma once
#include <vector>
#include "../num.h"

struct node_fb;

#define _nodeFuncParams_fb \
  node_fb *dest, uint destOffset, node_fb *src, uint globalIndexOffset
typedef void (*nodeFunc_fb)(_nodeFuncParams_fb);

struct node_fb {
  num      singleData;
  num     *inputData;
  uint     inputDataCount;
  node_fb *args;
  uint     argCount;
  nodeFunc_fb nf;
  node_fb(num literal);
  node_fb(nodeFunc_fb nfIn);
  node_fb(nodeFunc_fb nfIn, node_fb *argsIn, uint argCountIn);
  node_fb(nodeFunc_fb nfIn, std::vector<node_fb> &argsIn);
  node_fb(const node_fb &n);
  ~node_fb();
  void output(node_fb *dest, uint destOffset, uint globalIndexOffset);
  void outputTo(num *destData, uint destDataSize);
  void outputTo(std::vector<num> &destData);
};


void nf_add(_nodeFuncParams_fb);
void nf_sub(_nodeFuncParams_fb);
void nf_mul(_nodeFuncParams_fb);
void nf_div(_nodeFuncParams_fb);
void nf_literal(_nodeFuncParams_fb);
void nf_globalIndex(_nodeFuncParams_fb);
void nf_dummy(_nodeFuncParams_fb);



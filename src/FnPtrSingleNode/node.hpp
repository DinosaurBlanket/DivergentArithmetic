#pragma once
#include <vector>
#include "../num.h"

struct node_fs;

#define _nodeFuncParams_fs \
  node_fs *dest, uint destOffset, node_fs *src, uint globalIndexOffset
typedef void (*nodeFunc_fs)(_nodeFuncParams_fs);

const uint maxArgs = 2;

struct node_fs {
  num      inputData[maxArgs];
  node_fs *args;
  uint     argCount;
  nodeFunc_fs nf;
  node_fs(num literal);
  node_fs(nodeFunc_fs nfIn);
  node_fs(nodeFunc_fs nfIn, node_fs *argsIn, uint argCountIn);
  node_fs(nodeFunc_fs nfIn, std::vector<node_fs> &argsIn);
  node_fs(const node_fs &n);
  ~node_fs();
  void output(node_fs *dest, uint destOffset, uint globalIndexOffset);
  void outputTo(num *destData, uint destDataSize);
  void outputTo(std::vector<num> &destData);
};


void fs_add(_nodeFuncParams_fs);
void fs_sub(_nodeFuncParams_fs);
void fs_mul(_nodeFuncParams_fs);
void fs_div(_nodeFuncParams_fs);
void fs_literal(_nodeFuncParams_fs);
void fs_globalIndex(_nodeFuncParams_fs);
void fs_dummy(_nodeFuncParams_fs);



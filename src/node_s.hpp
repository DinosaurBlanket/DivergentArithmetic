#pragma once
#include <vector>
#include "num.h"

enum nodeIden {
  ni_literal,
  ni_add,
  ni_sub,
  ni_mul,
  ni_div,
  ni_globalIndex,
  ni_dummy,
  nodeIdenCount
};


class node_s {
  num      singleData;
  num     *inputData;
  uint     inputDataCount;
  node_s  *args;
  uint     argCount;
  nodeIden ni;
public:
  node_s(num literal);
  node_s(nodeIden niIn);
  node_s(nodeIden niIn, node_s *argsIn, uint argCountIn);
  node_s(nodeIden niIn, std::vector<node_s> &argsIn);
  node_s(const node_s &n);
  ~node_s();
  void output(node_s *dest, uint destOffset, uint globalIndexOffset);
  void outputTo(num *destData, uint destDataSize);
  void outputTo(std::vector<num> &destData);
};




#pragma once
#include <vector>
#include "../num.h"

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


class node_sb {
  num   singleData;
  num  *inputData;
  uint  inputDataCount;
  node_sb *args;
  uint  argCount;
  nodeIden ni;
public:
  node_sb(num literal);
  node_sb(nodeIden niIn);
  node_sb(nodeIden niIn, node_sb *argsIn, uint argCountIn);
  node_sb(nodeIden niIn, std::vector<node_sb> &argsIn);
  node_sb(const node_sb &n);
  ~node_sb();
  void output(node_sb *dest, uint destOffset, uint globalIndexOffset);
  void outputTo(num *destData, uint destDataSize);
  void outputTo(std::vector<num> &destData);
};




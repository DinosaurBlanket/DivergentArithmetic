
#include <vector>
#include "num.h"
typedef unsigned int uint;

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
};

void outputTo(node &root, num *destData, uint destDataSize);
void outputTo(node &root, std::vector<num> &destData);

void nf_add(_nodeFuncParams);
void nf_mul(_nodeFuncParams);
void nf_literal(_nodeFuncParams);
void nf_globalIndex(_nodeFuncParams);
void nf_dummy(_nodeFuncParams);



















/*
class dvrgArithNode;

#define _nodeFuncParams dvrgArithNode &dest, uint destOffset, dvrgArithNode &src
typedef void (*nodeFunc)(_nodeFuncParams);

class dvrgArithNode {
  vector<num> inputData;
  nodeFunc    nf;
  vector<dvrgArithNode> args;
public:
  dvrgArithNode();
  dvrgArithNode(nodeFunc nfIn, vector<dvrgArithNode> &argsIn);
  dvrgArithNode(nodeFunc nfIn);
  dvrgArithNode(num literal);
  void resetArgs(vector<dvrgArithNode> &argsIn);
  void resetnodeFunc(nodeFunc nfIn);
  void output(_nodeFuncParams);
};

void setPacketSize(uint newSize);
*/
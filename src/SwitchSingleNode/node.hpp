
#include "../num.h"


const uint ssn_maxArgs = 2;
struct ssn_node {
  num  ret;
  Iden iden;
  uint args[ssn_maxArgs];
};

void ssn_outputTo(num *data, uint dataSize, ssn_node *na, uint nodeCount);


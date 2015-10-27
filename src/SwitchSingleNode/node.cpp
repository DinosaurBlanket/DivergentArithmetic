
#include <iostream>
using std::cout;
using std::endl;
#include "node.hpp"

uint index;

#define _theSwitch \
switch (na[n].iden) {\
  case I_lit: ret = na[n].ret;break;\
  case I_add: ret = na[na[n].args[0]].ret + na[na[n].args[1]].ret;break;\
  case I_sub: ret = na[na[n].args[0]].ret - na[na[n].args[1]].ret;break;\
  case I_mul: ret = na[na[n].args[0]].ret * na[na[n].args[1]].ret;break;\
  case I_div: ret = na[na[n].args[0]].ret / na[na[n].args[1]].ret;break;\
  case I_index: na[n].ret = ret = index;break;\
  default: ret = -100;\
}

void ssn_outputTo(num *data, uint dataSize, ssn_node *na, uint nodeCount) {
  for (index = 0; index < dataSize; index++) {
    num ret;
    uint n = 0;
    _theSwitch
    while (n < nodeCount-1) {
      na[n].ret = ret;
      n++;
      _theSwitch
    }
    data[index] = ret;
  }
}



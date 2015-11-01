
#include <iostream>
using std::cout;
using std::endl;
#include "node.hpp"


void ssnb_outputTo(
  num  *data, uint dataSize,
  Iden *na,   uint nodeCount,
  num  *lits, uint litCount
) {
  num stack[nodeCount];
  for (uint index = 0; index < dataSize; index++) {
    uint S = 0, n = 0, l = 0;
    for (; n < nodeCount; n++) {
      switch (na[n]) {
        case I_lit: stack[n] = lits[l++]; break;
        case I_add: stack[n] = stack[S+1] + stack[S]; S+=2; break;
        case I_sub: stack[n] = stack[S+1] - stack[S]; S+=2; break;
        case I_mul: stack[n] = stack[S+1] * stack[S]; S+=2; break;
        case I_div: stack[n] = stack[S+1] / stack[S]; S+=2; break;
        case I_index: stack[n] = index; break;
        default: stack[n] = -100;
      }
    }
    data[index] = stack[nodeCount-1];
  }
}




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
    uint s = 0, S = 0, n = 0, l = 0;
    for (; n < nodeCount; n++) {
      switch (na[n]) {
        case I_lit: stack[s] = lits[l++]; break;
        case I_add: stack[s] = stack[S+1] + stack[S]; S+=2; break;
        case I_sub: stack[s] = stack[S+1] - stack[S]; S+=2; break;
        case I_mul: stack[s] = stack[S+1] * stack[S]; S+=2; break;
        case I_div: stack[s] = stack[S+1] / stack[S]; S+=2; break;
        case I_index: stack[s] = index; break;
        default: stack[s] = -100;
      }
      s++;
    }
    data[index] = stack[s-1];
  }
}



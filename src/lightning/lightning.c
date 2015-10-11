
#include <stdio.h>
#include <lightning.h>
#include "../num.h"

typedef void (*vff)(num *data, uint dataSize);

static jit_state_t *_jit;
jit_node_t  *data;
jit_node_t  *dataSize;
vff          lightningOut;

void initLightning() {
  init_jit(0);
  _jit = jit_new_state();
  jit_prolog();
  data = jit_arg();
  jit_getarg(JIT_R0, data);
  dataSize = jit_arg();
  jit_getarg(JIT_R1, dataSize);
  
  jit_ldr_f(JIT_F0, JIT_R0);
  jit_addi_f(JIT_F0, JIT_F0, 1.0);
  jit_str_f(JIT_R0, JIT_F0);
  
  lightningOut = jit_emit();
  jit_clear_state();
}

void runLightning(num *data, uint dataSize) {
  lightningOut(data, dataSize);
}

void cleanupLighntning() {
  jit_destroy_state();
  finish_jit();
}



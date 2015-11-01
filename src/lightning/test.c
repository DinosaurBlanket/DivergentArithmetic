#include <stdio.h>
#include <lightning.h>
#include "../num.h"

typedef void (*v_fp_ui)(num*, uint);

static jit_state_t *_jit;
jit_node_t  *data;
jit_node_t  *dataSize;
v_fp_ui      lightningOut;
jit_node_t  *loopBgn;
jit_node_t  *loopEnd;

void initLightning() {
  init_jit(0);
  _jit = jit_new_state();
  jit_prolog();
  
  data = jit_arg();
  jit_getarg(JIT_R0, data);
  dataSize = jit_arg();
  jit_getarg(JIT_R1, dataSize);
  
  jit_movi  (JIT_R2, 0);
  
  loopBgn = jit_label();
  
  jit_extr_f(JIT_F0, JIT_R2);          //convert counter to float
  jit_str_f (JIT_R0, JIT_F0);
  
  jit_addi  (JIT_R0, JIT_R0, 4);       //increment pointer (4 bytes)
  jit_addi  (JIT_R2, JIT_R2, 1);       //increment counter (1 element)
  
  loopEnd = jit_bltr(JIT_R2, JIT_R1);
  jit_patch_at(loopEnd, loopBgn);
  
  
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




#include <stdio.h>
#include <lightning.h>

typedef float (*pifi)(float);

static jit_state_t *_jit;
jit_node_t  *in;
pifi         incr;

void initLightning() {
  init_jit(0);
  _jit = jit_new_state();
  jit_prolog();
  in = jit_arg_f();
  jit_getarg_f(JIT_F0, in);
  jit_addi_f(JIT_F0, JIT_F0, 1.0);
  jit_retr_f(JIT_F0);
  incr = jit_emit();
  jit_clear_state();
}

void runLightning() {
  printf("%f + 1.0 = %f\n", 5.5, incr(5.5));
}

void cleanupLighntning() {
  jit_destroy_state();
  finish_jit();
}



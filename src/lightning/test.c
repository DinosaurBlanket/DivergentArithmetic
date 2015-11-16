#include <stdio.h>
#include <lightning.h>
#include "../num.h"

typedef void (*v_fp_ui)(num*, uint);


num _add(num a, num b) {return a + b;}
num _sub(num a, num b) {return a - b;}
num _mul(num a, num b) {return a * b;}
num _div(num a, num b) {return a / b;}


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
  jit_getarg     (JIT_R0, data);
  dataSize = jit_arg();
  jit_getarg     (JIT_R1, dataSize);
  
  jit_movi       (JIT_R2, 0);
  
  loopBgn = jit_label();
  
  jit_extr_f     (JIT_F0, JIT_R2);        //convert counter to float
  /*
  jit_prepare();
  jit_pushargr_f (JIT_F0);                //pass counter as LH argument
  jit_pushargi_f (2.0);                   //pass num lit 2 as RH argument
  jit_finishi    (_div);                  //call _div
  jit_retval_f   (JIT_F2);                //put ret val of _div in F2
  
  jit_prepare();
  jit_pushargi_f (2.0);
  jit_pushargr_f (JIT_F2);
  jit_finishi    (_mul);
  jit_retval_f   (JIT_F0);   //change to F1 for LH side
  */
  jit_str_f      (JIT_R0, JIT_F0);        //store the result
  
  
  
  jit_addi       (JIT_R0, JIT_R0, 4);     //increment pointer (4 bytes)
  jit_addi       (JIT_R2, JIT_R2, 1);     //increment counter (1 element)
  
  jit_pushargi((jit_word_t)"A");
  jit_finishi(puts);
  
  loopEnd = jit_bltr(JIT_R2, JIT_R1);
  jit_patch_at(loopEnd, loopBgn);
  
  
  //jit_pushargi((jit_word_t)"R2: %d\n");
  //jit_ellipsis();
  //jit_pushargr(JIT_R2);
  //jit_finishi(printf);
  
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



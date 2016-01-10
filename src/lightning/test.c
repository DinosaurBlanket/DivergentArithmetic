
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <lightning.h>


#define minFloatReg 2
#define maxArgs 2
typedef struct jitput jitput;
typedef struct node node;
typedef struct timespec timespec;

typedef union {
  float f;
  uint8_t b[4];
} value;
#define putasmParams  node* self, value *contextual
typedef void (*putasm)(putasmParams);
typedef void (*targetFn)(void);

struct jitput {
  putasm jitit;
  uint32_t   workMemSize;//in values
};
struct node {
  jitput jitme;
  value *workMem;//first value is return value
  node  *args[maxArgs];
  value  litVal;
};

float dataIndex = 0;

static jit_state_t *_jit;


void pa_lit(putasmParams) {}
const jitput jp_lit = {pa_lit, 1};

#define _binop(pa_name, jp_name, jit_op) \
  void pa_name(putasmParams) {\
    jit_ldi_f (JIT_F0, self->args[0]->workMem);\
    jit_ldi_f (JIT_F1, self->args[1]->workMem);\
    jit_op    (JIT_F0, JIT_F0, JIT_F1);\
    jit_sti_f (self->workMem, JIT_F0);\
  }\
  const jitput jp_name = {pa_name, 1}
;

_binop(pa_add, jp_add, jit_addr_f);
_binop(pa_sub, jp_sub, jit_subr_f);
_binop(pa_mul, jp_mul, jit_mulr_f);
_binop(pa_div, jp_div, jit_divr_f);



void pa_dataIndex(putasmParams) {
  jit_ldi_f (JIT_F0, &dataIndex);
  jit_sti_f (self->workMem, JIT_F0);
}
const jitput jp_dataIndex = {pa_dataIndex, 1};


const jitput jp_null = {NULL, 0};


long tsDiff_usec(timespec *a, timespec *b) {
  return (b->tv_sec - a->tv_sec)*1e6 + (b->tv_nsec - a->tv_nsec)/1e3;
}


int lightningTest(float *data, uint32_t dataSize) {
  if (JIT_F_NUM < minFloatReg) {
    printf(
      "platform not supported: %d float registers required, %d detected\n",
      minFloatReg, JIT_F_NUM
    );
    return -1;
  }
  
  
  /*
  add         8
    i         7
    sub       6
      mul     5
        2     4
        div   3
          i   2
          2   1
      i       0
  */
  node theNodes[] = {
    {.jitme = jp_dataIndex},                                  //0
    {.jitme = jp_lit, .litVal.f = 2},                         //1
    {.jitme = jp_dataIndex},                                  //2
    {.jitme = jp_div, .args = {&theNodes[2], &theNodes[1]}},  //3
    {.jitme = jp_lit, .litVal.f = 2},                         //4
    {.jitme = jp_mul, .args = {&theNodes[4], &theNodes[3]}},  //5
    {.jitme = jp_sub, .args = {&theNodes[5], &theNodes[0]}},  //6
    {.jitme = jp_dataIndex},                                  //7
    {.jitme = jp_add, .args = {&theNodes[7], &theNodes[6]}},  //8
    {.jitme = jp_null}                                        //9
  };
  
  uint32_t protoNodeCount = 0;
  uint32_t protoMempoolSize = 0;
  for (; theNodes[protoNodeCount].jitme.jitit; protoNodeCount++) {
    protoMempoolSize += theNodes[protoNodeCount].jitme.workMemSize;
  }
  const uint32_t nodeCount   = protoNodeCount;
  const uint32_t memPoolSize = protoMempoolSize;
  
  value memPool[memPoolSize];
  for (
    uint32_t n = 0, m = 0;
    n < nodeCount;
    n++, m += theNodes[n].jitme.workMemSize
  ) {
    theNodes[n].workMem = &memPool[m];
    theNodes[n].workMem[0] = theNodes[n].litVal;
  }
  
  
  timespec tsStart, tsEnd;
  clock_gettime(CLOCK_MONOTONIC, &tsStart);
  
  
  init_jit(0);
  _jit = jit_new_state();
  jit_prolog();
  
  for (uint32_t i = 0; i < nodeCount; i++) {
    theNodes[i].jitme.jitit(&theNodes[i], NULL);
  }
  
  targetFn lightningOut = jit_emit();
  jit_clear_state();
  
  clock_gettime(CLOCK_MONOTONIC, &tsEnd);
  ulong compileTime_usec = tsDiff_usec(&tsStart, &tsEnd);
  printf("compiled in %lu microseconds\n", compileTime_usec);
  
  
  clock_gettime(CLOCK_MONOTONIC, &tsStart);
  for (dataIndex = 0; dataIndex < dataSize; dataIndex++) {
    lightningOut();
    data[(int)dataIndex] = theNodes[0].workMem[0].f;
    printf("\n%f\n", dataIndex);
    for (uint i = 0; i < nodeCount; i++) {
      printf("\ttheNodes[%d].workMem[0].f: %f\n", i, theNodes[i].workMem[0].f);
    }
  }
  clock_gettime(CLOCK_MONOTONIC, &tsEnd);
  
  struct runTime_tv;
  ulong runTime_usec = tsDiff_usec(&tsStart, &tsEnd);
  
  printf("ran in %lu microseconds\n", runTime_usec);
  //puts("data:");
  //for (uint32_t i = 0; i < dataSize; i++) {
  //  printf("%3d: %6.2f\n", i, data[i]);
  //}
  
  
  jit_destroy_state();
  finish_jit();
  return 0;
}

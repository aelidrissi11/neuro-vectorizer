#include "header.h"

short in1[2048];
short in2[2048];
short in3[2048];
int   ia[2048] ALIGNED16;
int   ib[2048] ALIGNED16;
int   ic[2048] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ in1, short *__restrict__ in2, short *__restrict__ in3, int* __restrict__ ia, int* __restrict__ ib, int* __restrict__ ic) {
  int i;
  for (i = 0; i < 2048-1; i+=2) {
    ia[i] = (int) in1[i];
ia[i+1] = (int) in1[i+1];
    ib[i] = (int) in2[i];
ib[i+1] = (int) in2[i+1];
    ic[i] = (int) in3[i];
ic[i+1] = (int) in3[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[2048]);
  init_memory(&ib[0], &ib[2048]);
  init_memory(&ic[0], &ic[2048]);
  init_memory(&in1[0], &in1[2048]);
  init_memory(&in2[0], &in2[2048]);
  init_memory(&in3[0], &in3[2048]);
  BENCH("Example10b", example10b(in1,in2,in3,ia,ib,ic), Mi*4/2048*512, digest_memory(&ia[0], &ia[2048])+digest_memory(&ib[0], &ib[2048])+digest_memory(&ic[0], &ic[2048]));
 
  return 0;
}
#include "header.h"

short sa[64];
int   result[64] ALIGNED16;

__attribute__((noinline))
void example10b(short *__restrict__ sa, int* __restrict__ result) {
  int i;
  for (i = 0; i < 64-1; i+=2) {
    result[i] = (int) sa[i];
result[i+1] = (int) sa[i+1];
  }
}
int main(int argc,char* argv[]){
  init_memory(&result[0], &result[64]);
  init_memory(&sa[0], &sa[64]);
  BENCH("Example10b", example10b(sa,result), Mi*4/64*512, digest_memory(&result[0], &result[64]));
 
  return 0;
}
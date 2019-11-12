#include "header.h"

short sa[512];
short sb[512];
short sc[512];
int   ia[512] ALIGNED16;
int   ib[512] ALIGNED16;
int   ic[512] ALIGNED16;

__attribute__((noinline))
void example10a(short *__restrict__ sa, short *__restrict__ sb, short *__restrict__ sc, int* __restrict__ ia, int* __restrict__ ib, int* __restrict__ ic) {
  int i;
  for (i = 0; i < 512; i++) {
    ia[i] = ib[i] + ic[i];
    sa[i] = sb[i] + sc[i];
  }
}
int main(int argc,char* argv[]){
  init_memory(&ia[0], &ia[512]);
  init_memory(&ib[0], &ib[512]);
  init_memory(&ic[0], &ic[512]);
  init_memory(&sa[0], &sa[512]);
  init_memory(&sb[0], &sb[512]);
  init_memory(&sc[0], &sc[512]);
  BENCH("Example10a", example10a(sa,sb,sc,ia,ib,ic), Mi/512*512, digest_memory(&ia[0], &ia[512]) + digest_memory(&sa[0], &sa[512]));
 
  return 0;
}